#ifndef CHESS_ENGINE_CHESSENGINE_HPP
#define CHESS_ENGINE_CHESSENGINE_HPP

#include <memory>
#include "../core/Move.hpp"

namespace chess::core {
    class Game;
}

namespace chess::engine {

/**
 * Abstract interface for chess engines.
 * Implementations can be bots at various difficulty levels or external engines (e.g., Stockfish).
 */
class ChessEngine {
public:
    virtual ~ChessEngine() = default;

    /**
     * Returns the best move for the current game position.
     * The engine should analyze the current board and return a legal move.
     *
     * @param game the current game state
     * @return the move the engine wants to play
     * @throws std::runtime_error if no legal move can be found
     */
    virtual chess::core::Move getBestMove(const chess::core::Game& game) = 0;
};

}  // namespace chess::engine

#endif  // CHESS_ENGINE_CHESSENGINE_HPP
