#include "AlgebraicNotationUtil.hpp"
#include "../pieces/Pawn.hpp"
#include "../pieces/Knight.hpp"
#include "../pieces/Bishop.hpp"
#include "../pieces/Rook.hpp"
#include "../pieces/Queen.hpp"
#include "../pieces/King.hpp"
#include <sstream>
#include <algorithm>

namespace chess::util {

std::string AlgebraicNotationUtil::toLongAlgebraic(const chess::core::Move& move) {
    return move.getFrom().toAlgebraic() + move.getTo().toAlgebraic();
}

char AlgebraicNotationUtil::getPieceSymbol(const chess::core::Piece* piece) {
    if (!piece) return ' ';

    std::string name = piece->getPieceName();
    if (name == "Pawn") return 'P';
    if (name == "Knight") return 'N';
    if (name == "Bishop") return 'B';
    if (name == "Rook") return 'R';
    if (name == "Queen") return 'Q';
    if (name == "King") return 'K';
    return '?';
}

std::string AlgebraicNotationUtil::toAlgebraic(const chess::core::Board& /* board */,
                                                const chess::core::Move& move) {
    std::ostringstream oss;

    chess::core::Piece* piece = move.getMovedPiece();
    if (!piece) {
        return toLongAlgebraic(move);
    }

    std::string pieceName = piece->getPieceName();

    // Pawns are special: they use the destination square or capture notation
    if (pieceName == "Pawn") {
        if (move.isCapture_()) {
            // Pawn capture: e.g., "exd5"
            char fromFile = static_cast<char>('a' + move.getFrom().getFile());
            oss << fromFile << "x" << move.getTo().toAlgebraic();
        } else {
            // Pawn move: just the destination square
            oss << move.getTo().toAlgebraic();
        }
        if (move.isPromotion_()) {
            // Add promotion piece: e.g., "e8=Q"
            char promoPiece = 'Q';  // Default to queen
            if (move.getPromotionTarget() == chess::core::Move::PromotionTarget::ROOK) {
                promoPiece = 'R';
            } else if (move.getPromotionTarget() == chess::core::Move::PromotionTarget::BISHOP) {
                promoPiece = 'B';
            } else if (move.getPromotionTarget() == chess::core::Move::PromotionTarget::KNIGHT) {
                promoPiece = 'N';
            }
            oss << "=" << promoPiece;
        }
    } else {
        // Other pieces: e.g., "Nf3", "Bxc5"
        char symbol = getPieceSymbol(piece);
        oss << symbol;

        if (move.isCapture_()) {
            oss << "x";
        }
        oss << move.getTo().toAlgebraic();
    }

    return oss.str();
}

std::unique_ptr<chess::core::Move> AlgebraicNotationUtil::parseMove(const std::string& notation,
                                                                      const chess::core::Board& board,
                                                                      chess::core::Color playerColor) {
    // Try to parse as long algebraic (e2e4)
    if (notation.length() == 4) {
        try {
            chess::core::Position from = chess::core::Position::fromAlgebraic(notation.substr(0, 2));
            chess::core::Position to = chess::core::Position::fromAlgebraic(notation.substr(2, 2));

            chess::core::Piece* piece = board.getPiece(from);
            if (!piece || piece->getColor() != playerColor) {
                return nullptr;
            }

            chess::core::Piece* captured = board.getPiece(to);

            auto builder = chess::core::Move::Builder(from, to, piece);
            if (captured) {
                builder.withCapture(captured);
            }

            return std::make_unique<chess::core::Move>(builder.build());
        } catch (...) {
            // Fall through to other formats
        }
    }

    // Try to parse as coordinate notation with space (e2 e4)
    size_t spacePos = notation.find(' ');
    if (spacePos != std::string::npos) {
        try {
            chess::core::Position from = chess::core::Position::fromAlgebraic(notation.substr(0, spacePos));
            chess::core::Position to = chess::core::Position::fromAlgebraic(notation.substr(spacePos + 1));

            chess::core::Piece* piece = board.getPiece(from);
            if (!piece || piece->getColor() != playerColor) {
                return nullptr;
            }

            chess::core::Piece* captured = board.getPiece(to);

            auto builder = chess::core::Move::Builder(from, to, piece);
            if (captured) {
                builder.withCapture(captured);
            }

            return std::make_unique<chess::core::Move>(builder.build());
        } catch (...) {
            return nullptr;
        }
    }

    return nullptr;
}

}  // namespace chess::util
