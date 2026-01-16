#ifndef CHESS_CORE_GAMESTATE_HPP
#define CHESS_CORE_GAMESTATE_HPP

namespace chess::core {

/**
 * Enum representing the state of a chess game.
 */
enum class GameState {
    ONGOING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW_BY_AGREEMENT,
    RESIGNATION,
    TIME_OUT
};

}  // namespace chess::core

#endif  // CHESS_CORE_GAMESTATE_HPP
