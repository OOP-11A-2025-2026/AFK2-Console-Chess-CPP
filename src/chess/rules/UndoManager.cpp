#include "UndoManager.hpp"

namespace chess::rules {

using namespace chess::core;

UndoManager::UndoManager() {}

void UndoManager::saveSnapshot(const Game& game) {
    GameSnapshot snapshot;
    snapshot.boardState = std::make_unique<Board>(game.getBoard());
    snapshot.currentPlayer = game.getCurrentPlayerColor();
    snapshot.gameState = game.getGameState();
    snapshot.moveHistory = game.getMoveHistory();

    history.push_back(std::move(snapshot));
}

bool UndoManager::canUndo() const {
    return !history.empty();
}

GameSnapshot UndoManager::popSnapshot() {
    if (history.empty()) {
        throw std::runtime_error("No moves to undo");
    }

    GameSnapshot snapshot = std::move(history.back());
    history.pop_back();
    return snapshot;
}

void UndoManager::clear() {
    history.clear();
}

int UndoManager::getHistorySize() const {
    return history.size();
}

}
