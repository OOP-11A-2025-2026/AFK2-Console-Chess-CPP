#ifndef CHESS_IO_COMMANDHANDLER_HPP
#define CHESS_IO_COMMANDHANDLER_HPP

#include "CommandType.hpp"
#include "InputParser.hpp"
#include "ConsoleUI.hpp"
#include "../engine/GameController.hpp"
#include "../core/Move.hpp"
#include <memory>
#include <string>

namespace chess::io {

class CommandHandler {
private:
    chess::engine::GameController& gameController;

public:
    CommandHandler(chess::engine::GameController& controller);

    void handleCommand(const ParsedCommand& command);

private:
    void handleMove(const ParsedCommand& command);

    void handleNewGame();

    void handleSave(const std::string& filename);

    void handleLoad(const std::string& filename);

    void handleResign();

    void handleDrawOffer();

    void handleDrawAccept();

    void handleUndo();

    void handleHelp();
};

}

#endif
