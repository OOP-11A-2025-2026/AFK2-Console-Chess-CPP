#ifndef CHESS_UTIL_ALGEBRAICNOTATIONUTIL_HPP
#define CHESS_UTIL_ALGEBRAICNOTATIONUTIL_HPP

#include "../core/Move.hpp"
#include "../core/Board.hpp"
#include <string>

namespace chess::util {

/**
 * Utility class for converting moves to/from algebraic notation.
 */
class AlgebraicNotationUtil {
public:
    /**
     * Converts a move to long algebraic notation (e.g., "e2e4").
     * 
     * @param move the move to convert
     * @return the move in long algebraic notation
     */
    static std::string toLongAlgebraic(const chess::core::Move& move);

    /**
     * Converts a move to standard algebraic notation (e.g., "Nf3").
     * This is a simplified version - full SAN with disambiguation not yet implemented.
     * 
     * @param board the board state
     * @param move the move to convert
     * @return the move in algebraic notation
     */
    static std::string toAlgebraic(const chess::core::Board& board,
                                    const chess::core::Move& move);

    /**
     * Parses a move from algebraic notation.
     * Supports both long algebraic (e2e4) and coordinate notation (e2 e4).
     * 
     * @param notation the move notation
     * @param board the board state
     * @param playerColor the color of the player making the move
     * @return the parsed move, or nullptr if invalid
     */
    static std::unique_ptr<chess::core::Move> parseMove(const std::string& notation,
                                                         const chess::core::Board& board,
                                                         chess::core::Color playerColor);

private:
    /**
     * Gets the symbol for a piece type (N for knight, B for bishop, etc.)
     */
    static char getPieceSymbol(const chess::core::Piece* piece);
};

}  // namespace chess::util

#endif  // CHESS_UTIL_ALGEBRAICNOTATIONUTIL_HPP
