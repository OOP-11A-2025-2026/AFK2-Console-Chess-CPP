#ifndef CHESS_RULES_UNDOMANAGER_HPP
#define CHESS_RULES_UNDOMANAGER_HPP

#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include "../core/Game.hpp"
#include <vector>
#include <memory>

namespace chess::rules {

struct GameSnapshot {
    std::unique_ptr<chess::core::Board> boardState;
    chess::core::Color currentPlayer;
    chess::core::GameState gameState;
    std::vector<chess::core::Move> moveHistory;
};

class UndoManager {
private:
    std::vector<GameSnapshot> history;

public:
    UndoManager();

    void saveSnapshot(const chess::core::Game& game);

    bool canUndo() const;

    GameSnapshot popSnapshot();

    void clear();

    int getHistorySize() const;
};

}

#endif
