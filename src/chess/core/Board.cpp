#include "Board.hpp"
#include "../pieces/Pawn.hpp"
#include "../pieces/Rook.hpp"
#include "../pieces/Knight.hpp"
#include "../pieces/Bishop.hpp"
#include "../pieces/Queen.hpp"
#include "../pieces/King.hpp"

namespace chess::core {

Board::Board() : whiteKingPosition(4, 0), blackKingPosition(4, 7) {
    initializeStartingPosition();
}

Board::Board(const Board& other) 
    : whiteKingPosition(other.whiteKingPosition),
      blackKingPosition(other.blackKingPosition) {
    // Deep copy all pieces
    for (const auto& [pos, piece] : other.pieces) {
        pieces[pos] = piece->copy(pos);
    }
}

Board::Board(Board&& other) noexcept
    : pieces(std::move(other.pieces)),
      whiteKingPosition(other.whiteKingPosition),
      blackKingPosition(other.blackKingPosition) {
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        pieces.clear();
        // Deep copy all pieces
        for (const auto& [pos, piece] : other.pieces) {
            pieces[pos] = piece->copy(pos);
        }
        whiteKingPosition = other.whiteKingPosition;
        blackKingPosition = other.blackKingPosition;
    }
    return *this;
}

Board& Board::operator=(Board&& other) noexcept {
    if (this != &other) {
        pieces = std::move(other.pieces);
        whiteKingPosition = other.whiteKingPosition;
        blackKingPosition = other.blackKingPosition;
    }
    return *this;
}

void Board::initializeStartingPosition() {
    pieces.clear();

    // White pieces (rank 0)
    placePiece(std::make_unique<pieces::Rook>(Color::WHITE, Position(0, 0)), Position(0, 0));
    placePiece(std::make_unique<pieces::Knight>(Color::WHITE, Position(1, 0)), Position(1, 0));
    placePiece(std::make_unique<pieces::Bishop>(Color::WHITE, Position(2, 0)), Position(2, 0));
    placePiece(std::make_unique<pieces::Queen>(Color::WHITE, Position(3, 0)), Position(3, 0));
    placePiece(std::make_unique<pieces::King>(Color::WHITE, Position(4, 0)), Position(4, 0));
    whiteKingPosition = Position(4, 0);
    placePiece(std::make_unique<pieces::Bishop>(Color::WHITE, Position(5, 0)), Position(5, 0));
    placePiece(std::make_unique<pieces::Knight>(Color::WHITE, Position(6, 0)), Position(6, 0));
    placePiece(std::make_unique<pieces::Rook>(Color::WHITE, Position(7, 0)), Position(7, 0));

    // White pawns
    for (int file = 0; file < BOARD_SIZE; file++) {
        placePiece(std::make_unique<pieces::Pawn>(Color::WHITE, Position(file, 1)), Position(file, 1));
    }

    // Black pieces (rank 7)
    placePiece(std::make_unique<pieces::Rook>(Color::BLACK, Position(0, 7)), Position(0, 7));
    placePiece(std::make_unique<pieces::Knight>(Color::BLACK, Position(1, 7)), Position(1, 7));
    placePiece(std::make_unique<pieces::Bishop>(Color::BLACK, Position(2, 7)), Position(2, 7));
    placePiece(std::make_unique<pieces::Queen>(Color::BLACK, Position(3, 7)), Position(3, 7));
    placePiece(std::make_unique<pieces::King>(Color::BLACK, Position(4, 7)), Position(4, 7));
    blackKingPosition = Position(4, 7);
    placePiece(std::make_unique<pieces::Bishop>(Color::BLACK, Position(5, 7)), Position(5, 7));
    placePiece(std::make_unique<pieces::Knight>(Color::BLACK, Position(6, 7)), Position(6, 7));
    placePiece(std::make_unique<pieces::Rook>(Color::BLACK, Position(7, 7)), Position(7, 7));

    // Black pawns
    for (int file = 0; file < BOARD_SIZE; file++) {
        placePiece(std::make_unique<pieces::Pawn>(Color::BLACK, Position(file, 6)), Position(file, 6));
    }
}

Piece* Board::getPiece(const Position& pos) const {
    auto it = pieces.find(pos);
    if (it != pieces.end()) {
        return it->second.get();
    }
    return nullptr;
}

const Piece* Board::getPieceConst(const Position& pos) const {
    return getPiece(pos);
}

bool Board::isEmpty(const Position& pos) const {
    return getPiece(pos) == nullptr;
}

bool Board::isEnemyPiece(const Position& pos, Color color) const {
    Piece* piece = getPiece(pos);
    return piece != nullptr && piece->getColor() != color;
}

bool Board::isFriendlyPiece(const Position& pos, Color color) const {
    Piece* piece = getPiece(pos);
    return piece != nullptr && piece->getColor() == color;
}

void Board::placePiece(std::unique_ptr<Piece> piece, const Position& position) {
    if (!piece) {
        throw std::invalid_argument("Piece must not be null");
    }

    // Update king positions if needed
    if (piece->getPieceName() == "King") {
        if (piece->getColor() == Color::WHITE) {
            whiteKingPosition = position;
        } else {
            blackKingPosition = position;
        }
    }

    pieces[position] = std::move(piece);
}

std::unique_ptr<Piece> Board::removePiece(const Position& position) {
    auto it = pieces.find(position);
    if (it != pieces.end()) {
        auto piece = std::move(it->second);
        pieces.erase(it);
        return piece;
    }
    return nullptr;
}

std::unique_ptr<Piece> Board::movePiece(const Position& from, const Position& to) {
    Piece* piece = getPiece(from);
    if (!piece) {
        throw std::invalid_argument("No piece at position");
    }

    std::unique_ptr<Piece> capturedPiece = removePiece(to);
    auto movedPiece = removePiece(from);

    // Update the piece's position and mark it as moved
    movedPiece->setPosition(to);

    // Update king position if it's a king
    if (movedPiece->getPieceName() == "King") {
        if (movedPiece->getColor() == Color::WHITE) {
            whiteKingPosition = to;
        } else {
            blackKingPosition = to;
        }
    }

    placePiece(std::move(movedPiece), to);

    return capturedPiece;
}

Position Board::getKingPosition(Color color) const {
    return color == Color::WHITE ? whiteKingPosition : blackKingPosition;
}

std::vector<Piece*> Board::getPiecesOfColor(Color color) const {
    std::vector<Piece*> result;
    for (const auto& [pos, piece] : pieces) {
        if (piece && piece->getColor() == color) {
            result.push_back(piece.get());
        }
    }
    return result;
}

}  // namespace chess::core
