#include "MoveValidator.hpp"

namespace chess::rules {

bool MoveValidator::isValidMove(const chess::core::Board& board, const chess::core::Move& move,
                                chess::core::Color playerColor) {
    // Check piece at from position
    chess::core::Piece* piece = board.getPiece(move.getFrom());
    if (!piece) {
        return false;
    }

    // Check correct color
    if (piece->getColor() != playerColor) {
        return false;
    }

    // Check move is pseudo-legal
    const auto& destinations = piece->getLegalDestinations(board);
    bool isLegalDestination = std::find(destinations.begin(), destinations.end(),
                                       move.getTo()) != destinations.end();

    if (!isLegalDestination) {
        return false;
    }

    // Simulate the move and check if it leaves own king in check
    return !wouldLeaveKingInCheck(board, move, playerColor);
}

bool MoveValidator::isPositionAttacked(const chess::core::Board& board,
                                       const chess::core::Position& position,
                                       chess::core::Color enemyColor) {
    // Get all enemy pieces
    auto enemyPieces = board.getPiecesOfColor(enemyColor);

    // Check if any enemy piece can attack this position
    for (auto enemyPiece : enemyPieces) {
        if (!enemyPiece) continue;

        const auto& destinations = enemyPiece->getLegalDestinations(board);
        if (std::find(destinations.begin(), destinations.end(),
                     position) != destinations.end()) {
            return true;
        }
    }

    return false;
}

bool MoveValidator::wouldLeaveKingInCheck(const chess::core::Board& board,
                                          const chess::core::Move& move,
                                          chess::core::Color playerColor) {
    // Create a copy of the board
    chess::core::Board boardCopy = board;

    // Apply the move on the copy
    boardCopy.movePiece(move.getFrom(), move.getTo());

    // Check if the player's king is in check
    chess::core::Position kingPos = boardCopy.getKingPosition(playerColor);
    return isPositionAttacked(boardCopy, kingPos, opposite(playerColor));
}

}  // namespace chess::rules
