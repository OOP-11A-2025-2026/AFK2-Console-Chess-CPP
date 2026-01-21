#include <iostream>
#include <cassert>

#include "../../../src/chess/engine/GameController.hpp"
#include "../../../src/chess/core/GameState.hpp"
#include "../../../src/chess/core/Color.hpp"

using chess::engine::GameController;
using chess::core::GameState;
using chess::core::Color;
using chess::core::Player;
using chess::core::Move;

inline void startNewGame(GameController& controller) {
    Player white("White", Color::WHITE, false);
    Player black("Black", Color::BLACK, false);
    controller.newGame(white, black);
}

inline void playMove(GameController& controller, const std::string& notation) {
    controller.makeMoveAlgebraic(notation);
}

inline void undoMove(GameController& controller) {
    controller.undo();
}

inline void offerDraw(GameController& controller) {
    controller.offerDraw();
}

inline void acceptDraw(GameController& controller) {
    controller.acceptDraw();
}

inline void resignGame(GameController& controller) {
    controller.resign(Color::BLACK);
}

void printTest(const std::string& name) { std::cout << "[TEST] " << name << " ... "; }
void pass() { std::cout << "OK\n"; }

void testMoveUndoFlow() {
    printTest("Move + Undo");
    GameController controller;
    startNewGame(controller);

    playMove(controller, "e2e4");
    undoMove(controller);

    assert(controller.getGame().getMoveHistory().empty());
    assert(controller.getGameState() == GameState::ONGOING);

    pass();
}

void testSaveLoadConsistency() {
    printTest("Save + Load");
    GameController controller;
    startNewGame(controller);

    playMove(controller, "e2e4");
    playMove(controller, "e7e5");

    controller.saveGame("test_game.pgn");

    GameController loaded;
    loaded.loadGame("test_game.pgn");

    assert(loaded.getGame().getMoveHistory().size() == controller.getGame().getMoveHistory().size());
    assert(loaded.getGameState() == GameState::ONGOING);

    pass();
}

void testDrawFlow() {
    printTest("Draw offer + accept");
    GameController controller;
    startNewGame(controller);

    offerDraw(controller);
    acceptDraw(controller);

    assert(controller.getGameState() == GameState::DRAW_BY_AGREEMENT);
    pass();
}

void testResignFlow() {
    printTest("Resign");
    GameController controller;
    startNewGame(controller);

    resignGame(controller);

    assert(controller.getGameState() == GameState::RESIGNATION);
    pass();
}

void testUndoRestoresTurn() {
    printTest("Undo restores turn");
    GameController controller;
    startNewGame(controller);

    Color before = controller.getGame().getCurrentTurn();
    playMove(controller, "e2e4");
    undoMove(controller);
    Color after = controller.getGame().getCurrentTurn();

    assert(before == after);
    pass();
}

void runGameControllerFlowTests() {
    //std::cout << "\n=== GameController FLOW TESTS ===\n";

    testMoveUndoFlow();
    testSaveLoadConsistency();
    testDrawFlow();
    testResignFlow();
    testUndoRestoresTurn();

    //std::cout << "=== ALL FLOW TESTS PASSED ===\n\n";
}