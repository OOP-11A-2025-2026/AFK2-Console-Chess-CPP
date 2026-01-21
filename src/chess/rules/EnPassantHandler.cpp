#include "EnPassantHandler.hpp"
#include "../pieces/Pawn.hpp"

namespace chess::rules {

using namespace chess::core;

bool EnPassantHandler::isValidEnPassant(const Board& board, const Move& move,
                                       Color playerColor, const Move* lastMove) {
    if (!move.getIsEnPassant()) {
        return false;
    }

    Piece* movingPiece = board.getPiece(move.getFrom());
    if (!movingPiece || movingPiece->getColor() != playerColor) {
        return false;
    }

    if (!lastMove) {
        return false;
    }

    Piece* lastMovedPiece = lastMove->getMovedPiece();
    if (!lastMovedPiece) {
        return false;
    }

    Position lastFrom = lastMove->getFrom();
    Position lastTo = lastMove->getTo();

    int fromRank = lastFrom.getRank();
    int toRank = lastTo.getRank();

    if (std::abs(fromRank - toRank) != 2) {
        return false;
    }

    if (lastTo.getFile() != move.getTo().getFile()) {
        return false;
    }

    int expectedRank = (playerColor == Color::WHITE) ? 4 : 3;
    if (move.getFrom().getRank() != expectedRank) {
        return false;
    }

    return true;
}

Position EnPassantHandler::getEnPassantTarget(const Move& pawnMove) {
    int fromRank = pawnMove.getFrom().getRank();
    int toRank = pawnMove.getTo().getRank();
    int file = pawnMove.getTo().getFile();

    int targetRank = (fromRank + toRank) / 2;
    return Position(file, targetRank);
}

void EnPassantHandler::applyEnPassant(Board& board, const Move& move) {
    Position from = move.getFrom();
    Position to = move.getTo();

    board.movePiece(from, to);

    int capturedRank = from.getRank();
    Position capturedPos(to.getFile(), capturedRank);
    board.removePiece(capturedPos);
}

}
