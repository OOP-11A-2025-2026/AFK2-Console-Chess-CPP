#include "CastlingHandler.hpp"
#include "../pieces/King.hpp"
#include "../pieces/Rook.hpp"

namespace chess::rules {

using namespace chess::core;

bool CastlingHandler::isValidCastle(const Board& board, const Move& move, Color playerColor) {
    if (!move.isCastling_()) {
        return false;
    }

    Piece* movingPiece = board.getPiece(move.getFrom());
    if (!movingPiece || movingPiece->getColor() != playerColor) {
        return false;
    }

    if (movingPiece->hasMoved_()) {
        return false;
    }

    Position kingTarget = move.getTo();
    int kingFile = kingTarget.getFile();

    if (kingFile == 6) {
        return canCastleKingSide(board, playerColor);
    } else if (kingFile == 2) {
        return canCastleQueenSide(board, playerColor);
    }

    return false;
}

bool CastlingHandler::canCastleKingSide(const Board& board, Color color) {
    int rank = (color == Color::WHITE) ? 0 : 7;

    Piece* king = board.getPiece(Position(4, rank));
    Piece* rook = board.getPiece(Position(7, rank));

    if (!king || !rook || king->hasMoved_() || rook->hasMoved_()) {
        return false;
    }

    if (!board.isEmpty(Position(5, rank)) || !board.isEmpty(Position(6, rank))) {
        return false;
    }

    return true;
}

bool CastlingHandler::canCastleQueenSide(const Board& board, Color color) {
    int rank = (color == Color::WHITE) ? 0 : 7;

    Piece* king = board.getPiece(Position(4, rank));
    Piece* rook = board.getPiece(Position(0, rank));

    if (!king || !rook || king->hasMoved_() || rook->hasMoved_()) {
        return false;
    }

    if (!board.isEmpty(Position(1, rank)) || !board.isEmpty(Position(2, rank)) ||
        !board.isEmpty(Position(3, rank))) {
        return false;
    }

    return true;
}

void CastlingHandler::applyCastling(Board& board, const Move& move) {
    Position kingFrom = move.getFrom();
    Position kingTo = move.getTo();

    Piece* king = board.getPiece(kingFrom);
    board.movePiece(kingFrom, kingTo);

    int rank = kingFrom.getRank();
    if (kingTo.getFile() == 6) {
        Piece* rook = board.getPiece(Position(7, rank));
        board.movePiece(Position(7, rank), Position(5, rank));
    } else if (kingTo.getFile() == 2) {
        Piece* rook = board.getPiece(Position(0, rank));
        board.movePiece(Position(0, rank), Position(3, rank));
    }
}

}
