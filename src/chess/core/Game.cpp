#include "Game.hpp"
#include <stdexcept>

namespace chess::core {

Game::Game(std::unique_ptr<Player> whitePlayer, std::unique_ptr<Player> blackPlayer,
           std::unique_ptr<ChessClock> clock)
    : whitePlayer(std::move(whitePlayer)),
      blackPlayer(std::move(blackPlayer)),
      clock(std::move(clock)),
      currentPlayer(Color::WHITE),
      gameState(GameState::ONGOING),
      drawOfferPending(false),
      drawOfferer(nullptr) {
    if (!this->whitePlayer || !this->blackPlayer) {
        throw std::invalid_argument("Players must not be null");
    }
    if (!this->clock) {
        // Default 5-minute clock (5 * 60 * 1000 milliseconds)
        this->clock = std::make_unique<ChessClock>(5 * 60 * 1000);
    }

    // Initialize board
    board = std::make_unique<Board>();

    // Start the clock for white
    this->clock->startTurn(Color::WHITE);
}

Player& Game::getCurrentPlayer() {
    return currentPlayer == Color::WHITE ? *whitePlayer : *blackPlayer;
}

const Player& Game::getCurrentPlayer() const {
    return currentPlayer == Color::WHITE ? *whitePlayer : *blackPlayer;
}

void Game::offerDraw(Color player) {
    if (gameState != GameState::ONGOING) {
        throw std::runtime_error("Cannot offer draw when game is not ongoing");
    }
    drawOfferPending = true;
    drawOfferer = new Color(player);
}

void Game::acceptDraw() {
    if (!drawOfferPending) {
        throw std::runtime_error("No draw offer to accept");
    }
    gameState = GameState::DRAW_BY_AGREEMENT;
    delete drawOfferer;
    drawOfferer = nullptr;
    drawOfferPending = false;
}

void Game::declineDraw() {
    drawOfferPending = false;
    if (drawOfferer) {
        delete drawOfferer;
        drawOfferer = nullptr;
    }
}

void Game::addMove(const Move& move) {
    moveHistory.push_back(move);
}

void Game::switchTurn() {
    // Stop current player's clock
    clock->stopTurn();

    // Switch player
    currentPlayer = opposite(currentPlayer);

    // Start new player's clock
    clock->startTurn(currentPlayer);

    // Clear draw offer on turn switch
    declineDraw();
}

void Game::reset() {
    board = std::make_unique<Board>();
    moveHistory.clear();
    currentPlayer = Color::WHITE;
    gameState = GameState::ONGOING;
    drawOfferPending = false;
    if (drawOfferer) {
        delete drawOfferer;
        drawOfferer = nullptr;
    }
    clock->reset();
    clock->startTurn(Color::WHITE);
}

}  // namespace chess::core
