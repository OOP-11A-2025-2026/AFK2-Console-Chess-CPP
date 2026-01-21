#include "PgnIO.hpp"
#include "../rules/MoveValidator.hpp"
#include "../util/AlgebraicNotationUtil.hpp"
#include "../pieces/Queen.hpp"
#include "../pieces/Rook.hpp"
#include "../pieces/Bishop.hpp"
#include "../pieces/Knight.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace chess::pgn {
namespace {

using chess::core::Board;
using chess::core::Color;
using chess::core::Game;
using chess::core::Move;
using chess::core::opposite;
using chess::core::Piece;
using chess::core::Position;

std::string trim(const std::string& value) {
    const auto start = value.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

bool parseTagLine(const std::string& line, std::string& key, std::string& value) {
    if (line.empty() || line.front() != '[') return false;
    auto closePos = line.find(']');
    if (closePos == std::string::npos) return false;

    auto spacePos = line.find(' ');
    if (spacePos == std::string::npos) return false;

    key = line.substr(1, spacePos - 1);
    auto quoteStart = line.find('"', spacePos);
    if (quoteStart == std::string::npos) return false;
    auto quoteEnd = line.find('"', quoteStart + 1);
    if (quoteEnd == std::string::npos) return false;

    value = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
    return true;
}

std::string stripDecorators(std::string token) {
    while (!token.empty()) {
        char last = token.back();
        if (last == '+' || last == '#' || last == '!' || last == '?') {
            token.pop_back();
        } else {
            break;
        }
    }
    return token;
}

bool isResultToken(const std::string& token) {
    return token == "1-0" || token == "0-1" || token == "1/2-1/2" || token == "*";
}

bool isMoveNumberToken(const std::string& token) {
    if (token.empty()) return false;
    size_t idx = 0;
    while (idx < token.size() && std::isdigit(static_cast<unsigned char>(token[idx]))) {
        ++idx;
    }
    if (idx == 0) return false;
    while (idx < token.size() && token[idx] == '.') {
        ++idx;
    }
    return idx == token.size();
}

std::unique_ptr<Piece> createPromotionPiece(Color color, Move::PromotionTarget target,
                                            const Position& pos) {
    using namespace chess::pieces;
    switch (target) {
        case Move::PromotionTarget::ROOK:
            return std::make_unique<Rook>(color, pos);
        case Move::PromotionTarget::BISHOP:
            return std::make_unique<Bishop>(color, pos);
        case Move::PromotionTarget::KNIGHT:
            return std::make_unique<Knight>(color, pos);
        case Move::PromotionTarget::QUEEN:
        default:
            return std::make_unique<Queen>(color, pos);
    }
}

bool matchesDisambiguation(const Position& pos, const std::string& disambiguation) {
    if (disambiguation.empty()) return true;
    if (disambiguation.size() == 1) {
        char hint = disambiguation[0];
        if (hint >= 'a' && hint <= 'h') {
            return pos.getFile() == hint - 'a';
        }
        if (hint >= '1' && hint <= '8') {
            return pos.getRank() == hint - '1';
        }
    }
    if (disambiguation.size() == 2) {
        try {
            return pos == Position::fromAlgebraic(disambiguation);
        } catch (...) {
            return false;
        }
    }
    return false;
}

bool pieceMatches(char pieceChar, const Piece* piece) {
    if (!piece) return false;
    const std::string name = piece->getPieceName();
    switch (pieceChar) {
        case 'K': return name == "King";
        case 'Q': return name == "Queen";
        case 'R': return name == "Rook";
        case 'B': return name == "Bishop";
        case 'N': return name == "Knight";
        case 'P': return name == "Pawn";
        default: return false;
    }
}

Move::PromotionTarget parsePromotionTarget(char promoChar) {
    switch (promoChar) {
        case 'R': return Move::PromotionTarget::ROOK;
        case 'B': return Move::PromotionTarget::BISHOP;
        case 'N': return Move::PromotionTarget::KNIGHT;
        case 'Q':
        default: return Move::PromotionTarget::QUEEN;
    }
}

void applyMoveToBoard(Board& board, const Move& move) {
    board.movePiece(move.getFrom(), move.getTo());

    // Handle rook movement for castling
    if (move.isCastling_()) {
        const int rank = move.getFrom().getRank();
        if (move.getTo().getFile() == 6) {  // kingside
            board.movePiece(Position(7, rank), Position(5, rank));
        } else if (move.getTo().getFile() == 2) {  // queenside
            board.movePiece(Position(0, rank), Position(3, rank));
        }
    }

    // Handle promotion replacement
    if (move.isPromotion_()) {
        auto movedPiece = board.removePiece(move.getTo());
        (void)movedPiece;
        auto promoted = createPromotionPiece(move.getMovedPiece()->getColor(),
                                             move.getPromotionTarget(),
                                             move.getTo());
        promoted->setPosition(move.getTo());
        board.placePiece(std::move(promoted), move.getTo());
    }
}

std::unique_ptr<Move> parseCastlingToken(const std::string& upperToken, Board& board,
                                         Color color) {
    const bool kingSide = (upperToken == "O-O" || upperToken == "0-0");
    const bool queenSide = (upperToken == "O-O-O" || upperToken == "0-0-0");
    if (!kingSide && !queenSide) {
        return nullptr;
    }

    Position kingFrom = board.getKingPosition(color);
    Position kingTo = kingSide
        ? Position(6, kingFrom.getRank())
        : Position(2, kingFrom.getRank());

    Piece* king = board.getPiece(kingFrom);
    if (!king || king->getPieceName() != "King") {
        return nullptr;
    }

    Move::Builder builder(kingFrom, kingTo, king);
    builder.withCastling();
    return std::make_unique<Move>(builder.build());
}

std::unique_ptr<Move> parseSanMove(const std::string& rawToken, Board& board, Color color) {
    const std::string tokenTrimmed = trim(rawToken);
    if (tokenTrimmed.empty()) return nullptr;

    std::string upperToken = tokenTrimmed;
    std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

    auto castleMove = parseCastlingToken(upperToken, board, color);
    if (castleMove) {
        return castleMove;
    }

    std::string token = stripDecorators(tokenTrimmed);

    Move::PromotionTarget promotion = Move::PromotionTarget::NONE;
    const auto promoPos = token.find('=');
    if (promoPos != std::string::npos && promoPos + 1 < token.size()) {
        promotion = parsePromotionTarget(static_cast<char>(std::toupper(token[promoPos + 1])));
        token = token.substr(0, promoPos);
    }

    // Identify destination square (use last occurrence)
    size_t destIndex = std::string::npos;
    for (size_t i = 0; i + 1 < token.size(); ++i) {
        if (token[i] >= 'a' && token[i] <= 'h' &&
            token[i + 1] >= '1' && token[i + 1] <= '8') {
            destIndex = i;
        }
    }
    if (destIndex == std::string::npos) {
        return nullptr;
    }

    std::string destStr = token.substr(destIndex, 2);
    Position to = Position::fromAlgebraic(destStr);
    token.erase(destIndex, 2);

    char pieceChar = 'P';
    if (!token.empty() && std::isupper(static_cast<unsigned char>(token[0]))) {
        pieceChar = token[0];
        token.erase(0, 1);
    }

    const bool isCapture = token.find('x') != std::string::npos;
    token.erase(std::remove(token.begin(), token.end(), 'x'), token.end());
    const std::string disambiguation = token;

    std::vector<Move> legalMoves;
    for (Piece* candidate : board.getPiecesOfColor(color)) {
        if (!pieceMatches(pieceChar, candidate)) continue;
        if (!matchesDisambiguation(candidate->getPosition(), disambiguation)) continue;

        const auto destinations = candidate->getLegalDestinations(board);
        if (std::find(destinations.begin(), destinations.end(), to) == destinations.end()) {
            continue;
        }

        Move::Builder builder(candidate->getPosition(), to, candidate);
        if (isCapture && board.isEnemyPiece(to, color)) {
            builder.withCapture(board.getPiece(to));
        }
        if (promotion != Move::PromotionTarget::NONE) {
            builder.withPromotion(promotion);
        }

        Move move = builder.build();
        if (chess::rules::MoveValidator::isValidMove(board, move, color)) {
            legalMoves.push_back(move);
        }
    }

    if (legalMoves.size() == 1) {
        return std::make_unique<Move>(legalMoves.front());
    }
    if (!legalMoves.empty() && !disambiguation.empty()) {
        return std::make_unique<Move>(legalMoves.front());
    }

    return nullptr;
}

std::string deriveResultToken(chess::core::GameState state) {
    switch (state) {
        case chess::core::GameState::CHECKMATE:
            return "1-0";  // Winner not tracked, but non-draw result recorded
        case chess::core::GameState::STALEMATE:
        case chess::core::GameState::DRAW_BY_AGREEMENT:
            return "1/2-1/2";
        default:
            return "*";
    }
}

void setGameStateFromToken(Game& game, const std::string& token) {
    if (token == "1/2-1/2") {
        game.setGameState(chess::core::GameState::STALEMATE);
    } else if (token == "1-0" || token == "0-1") {
        game.setGameState(chess::core::GameState::CHECKMATE);
    } else {
        game.setGameState(chess::core::GameState::ONGOING);
    }
}

}  // namespace

PgnLoadResult PgnIO::loadFromFile(const std::string& path, Game& game) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Failed to open PGN file: " + path);
    }

    game.reset();

    PgnLoadResult result;
    std::string movetext;
    std::string line;
    while (std::getline(in, line)) {
        std::string key, value;
        if (parseTagLine(line, key, value)) {
            result.tags[key] = value;
            continue;
        }
        if (!line.empty() && line[0] == ';') {
            continue;
        }
        movetext += line;
        movetext += ' ';
    }

    // Remove brace comments
    std::string cleaned;
    bool inBrace = false;
    for (char c : movetext) {
        if (c == '{') {
            inBrace = true;
            continue;
        }
        if (c == '}') {
            inBrace = false;
            continue;
        }
        if (inBrace) continue;
        cleaned.push_back(c);
    }

    std::istringstream iss(cleaned);
    std::string token;
    Color toMove = Color::WHITE;
    while (iss >> token) {
        token = trim(token);
        if (token.empty()) continue;
        if (isMoveNumberToken(token)) continue;
        if (isResultToken(token)) {
            result.resultToken = token;
            break;
        }

        auto move = parseSanMove(token, game.getBoard(), toMove);
        if (!move) {
            throw std::runtime_error("Unable to parse move token: " + token);
        }

        applyMoveToBoard(game.getBoard(), *move);
        game.addMove(*move);
        game.switchTurn();
        toMove = opposite(toMove);
    }

    if (result.resultToken.empty()) {
        result.resultToken = "*";
    }

    setGameStateFromToken(game, result.resultToken);
    return result;
}

void PgnIO::saveToFile(const std::string& path, const Game& game,
                       const std::unordered_map<std::string, std::string>& extraTags) {
    std::ofstream out(path);
    if (!out) {
        throw std::runtime_error("Failed to open PGN output file: " + path);
    }

    std::unordered_map<std::string, std::string> tags{
        {"Event", "AFK2 Console Chess"},
        {"White", game.getWhitePlayer().getName()},
        {"Black", game.getBlackPlayer().getName()},
        {"Result", deriveResultToken(game.getGameState())}
    };

    for (const auto& kv : extraTags) {
        tags[kv.first] = kv.second;
    }

    for (const auto& kv : tags) {
        out << "[" << kv.first << " \"" << kv.second << "\"]\n";
    }
    out << "\n";

    Board replayBoard;
    const auto& history = game.getMoveHistory();
    size_t moveNumber = 1;

    for (size_t idx = 0; idx < history.size(); ++idx) {
        const Move& recorded = history[idx];
        Piece* piece = replayBoard.getPiece(recorded.getFrom());
        if (!piece) {
            throw std::runtime_error("Move history is inconsistent with a fresh board");
        }

        Move::Builder builder(recorded.getFrom(), recorded.getTo(), piece);
        if (recorded.isCapture_()) {
            builder.withCapture(replayBoard.getPiece(recorded.getTo()));
        }
        if (recorded.isCastling_()) {
            builder.withCastling();
        }
        if (recorded.isEnPassant_()) {
            builder.withEnPassant();
        }
        if (recorded.isPromotion_()) {
            builder.withPromotion(recorded.getPromotionTarget());
        }
        Move move = builder.build();

        if (idx % 2 == 0) {
            out << moveNumber << ". ";
            ++moveNumber;
        }

        out << chess::util::AlgebraicNotationUtil::toAlgebraic(replayBoard, move) << ' ';
        applyMoveToBoard(replayBoard, move);
    }

    out << tags["Result"] << "\n";
}

}  // namespace chess::pgn
