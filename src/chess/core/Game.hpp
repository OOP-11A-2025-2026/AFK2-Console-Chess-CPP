#ifndef CHESS_CORE_GAME_HPP
#define CHESS_CORE_GAME_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "ChessClock.hpp"
#include "GameState.hpp"
#include "Move.hpp"
#include <vector>
#include <memory>

namespace chess::core {

/**
 * Represents a chess game, holding the board, players, and game history.
 */
class Game {
private:
    std::unique_ptr<Board> board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    std::unique_ptr<ChessClock> clock;

    std::vector<Move> moveHistory;
    Color currentPlayer;
    GameState gameState;
    bool drawOfferPending;
    Color* drawOfferer;  // nullptr if no draw offer

public:
    /**
     * Creates a new game with two players and a clock.
     * 
     * @param whitePlayer the white player
     * @param blackPlayer the black player
     * @param clock the chess clock (if nullptr, creates default 5-minute clock)
     */
    Game(std::unique_ptr<Player> whitePlayer, std::unique_ptr<Player> blackPlayer,
         std::unique_ptr<ChessClock> clock = nullptr);

    /**
     * Gets the chess board.
     */
    Board& getBoard() { return *board; }
    const Board& getBoard() const { return *board; }

    /**
     * Gets the white player.
     */
    Player& getWhitePlayer() { return *whitePlayer; }
    const Player& getWhitePlayer() const { return *whitePlayer; }

    /**
     * Gets the black player.
     */
    Player& getBlackPlayer() { return *blackPlayer; }
    const Player& getBlackPlayer() const { return *blackPlayer; }

    /**
     * Gets the current player whose turn it is.
     */
    Player& getCurrentPlayer();
    const Player& getCurrentPlayer() const;

    /**
     * Gets the color of the current player.
     */
    Color getCurrentPlayerColor() const { return currentPlayer; }

    /**
     * Gets the chess clock.
     */
    ChessClock& getClock() { return *clock; }
    const ChessClock& getClock() const { return *clock; }

    /**
     * Gets the current game state.
     */
    GameState getGameState() const { return gameState; }

    /**
     * Sets the game state.
     */
    void setGameState(GameState state) { this->gameState = state; }

    /**
     * Gets the move history.
     */
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }

    /**
     * Checks if there is a pending draw offer.
     */
    bool isDrawOfferPending() const { return drawOfferPending; }

    /**
     * Gets the player who offered the draw (nullptr if no offer).
     */
    Color* getDrawOfferer() const { return drawOfferer; }

    /**
     * Offers a draw.
     */
    void offerDraw(Color player);

    /**
     * Accepts a draw offer.
     */
    void acceptDraw();

    /**
     * Declines a draw offer.
     */
    void declineDraw();

    /**
     * Adds a move to the move history and switches turns.
     */
    void addMove(const Move& move);

    /**
     * Switches the current player.
     */
    void switchTurn();

    /**
     * Resets the game to initial state.
     */
    void reset();

    /**
     * Applies a move to the board and updates game state.
     * Handles validation and special moves (promotion, castling, en passant).
     * 
     * @param move the move to apply
     * @throws std::runtime_error if move is invalid
     */
    void applyMove(const Move& move);

    /**
     * Applies a move using algebraic notation.
     * Parses the notation and makes the move if valid.
     * 
     * @param notation the move in algebraic notation (e.g., "e2e4" or "Nf3")
     * @throws std::runtime_error if notation is invalid or move is illegal
     */
    void applyMoveAlgebraic(const std::string& notation);

    /**
     * Resigns the game for the specified player.
     * 
     * @param resigningColor the color of the player resigning
     */
    void resign(Color resigningColor);

    /**
     * Updates the game state based on current board position.
     * Checks for check, checkmate, stalemate, and time losses.
     */
    void updateState();

    /**
     * Gets the current game state.
     */
    GameState getState() const { return gameState; }
};

}  // namespace chess::core

#endif  // CHESS_CORE_GAME_HPP
