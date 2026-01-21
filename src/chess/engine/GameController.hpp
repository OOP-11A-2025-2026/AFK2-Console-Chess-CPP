#pragma once

#include <memory>
#include <string>

#include "../core/Game.hpp"
#include "../core/Player.hpp"
#include "../core/Move.hpp"
#include "../core/GameState.hpp"

#include "../pgn/PgnWriter.hpp"
#include "../pgn/PgnParser.hpp"

#include "../rules/UndoManager.hpp"

#include "ChessEngine.hpp"

namespace chess::engine {

class GameController {
private:
    std::unique_ptr<core::Game> game;
    std::unique_ptr<rules::UndoManager> undoManager;

    std::unique_ptr<ChessEngine> botEngine;
    bool botEnabled = false;

public:
    GameController();

    void newGame(const core::Player& white,
                 const core::Player& black);

    void loadGame(const std::string& pgnFile);
    void saveGame(const std::string& pgnFile) const;

    void makeMove(const core::Move& move);
    void makeMoveAlgebraic(const std::string& notation);

    void undo();
    void offerDraw();
    void acceptDraw();
    void resign(core::Color resigningColor);

    void enableBot(std::unique_ptr<ChessEngine> engine);
    void disableBot();
    void makeBotMove();

    const core::Game& getGame() const;
    core::GameState getGameState() const;

private:
    void ensureGameExists() const;
    void updateGameState();
};

}
