#ifndef CHESS_RULES_MOVEVALIDATOR_HPP
#define CHESS_RULES_MOVEVALIDATOR_HPP

#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include "../core/Color.hpp"

namespace chess::rules {

/**
 * Validates chess moves according to FIDE rules.
 */
class MoveValidator {
public:
    /**
     * Validates a move completely.
     * Checks: piece exists, is correct color, move is pseudo-legal, doesn't leave king in check.
     * 
     * @param board the current board state
     * @param move the move to validate
     * @param playerColor the color of the player making the move
     * @return true if the move is valid, false otherwise
     */
    static bool isValidMove(const chess::core::Board& board, const chess::core::Move& move,
                           chess::core::Color playerColor);

    /**
     * Checks if a position is attacked by enemy pieces.
     * 
     * @param board the board state
     * @param position the position to check
     * @param enemyColor the color of the attacking side
     * @return true if the position is attacked by an enemy piece
     */
    static bool isPositionAttacked(const chess::core::Board& board,
                                   const chess::core::Position& position,
                                   chess::core::Color enemyColor);

    /**
     * Checks if a move would leave the player's own king in check.
     * Simulates the move on a board copy and checks if the player's king remains safe.
     * 
     * @param board the current board state
     * @param move the move to simulate
     * @param playerColor the color of the player making the move
     * @return true if the move would leave own king in check, false if it's safe
     */
    static bool wouldLeaveKingInCheck(const chess::core::Board& board,
                                      const chess::core::Move& move,
                                      chess::core::Color playerColor);
};

}  // namespace chess::rules

#endif  // CHESS_RULES_MOVEVALIDATOR_HPP
