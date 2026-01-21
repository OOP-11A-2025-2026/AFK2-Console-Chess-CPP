#include "CommandHandler.hpp"

namespace chess::io {

using namespace chess::core;

CommandHandler::CommandHandler(chess::engine::GameController& controller)
    : gameController(controller) {}

void CommandHandler::handleCommand(const ParsedCommand& command) {
    switch (command.type) {
        case CommandType::MOVE:
            handleMove(command);
            break;
        case CommandType::NEW_GAME:
            handleNewGame();
            break;
        case CommandType::SAVE:
            handleSave(command.argument);
            break;
        case CommandType::LOAD:
            handleLoad(command.argument);
            break;
        case CommandType::RESIGN:
            handleResign();
            break;
        case CommandType::DRAW_OFFER:
            handleDrawOffer();
            break;
        case CommandType::DRAW_ACCEPT:
            handleDrawAccept();
            break;
        case CommandType::UNDO:
            handleUndo();
            break;
        case CommandType::HELP:
            handleHelp();
            break;
        case CommandType::EXIT:
            ConsoleUI::displayMessage("Goodbye!");
            std::exit(0);
        case CommandType::INVALID:
            ConsoleUI::displayError("Invalid command");
            break;
    }
}

void CommandHandler::handleMove(const ParsedCommand& command) {
    try {
        if (command.move.has_value()) {
            gameController.makeMove(command.move.value());
        } else if (!command.argument.empty()) {
            gameController.makeMoveAlgebraic(command.argument);
        } else {
            ConsoleUI::displayError("Invalid move");
        }
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleNewGame() {
    try {
        std::string whiteName, blackName;
        ConsoleUI::displayMessage("White player name:");
        ConsoleUI::displayPromptForInput();
        std::getline(std::cin, whiteName);

        ConsoleUI::displayMessage("Black player name:");
        ConsoleUI::displayPromptForInput();
        std::getline(std::cin, blackName);

        gameController.newGame(
            Player(whiteName, Color::WHITE, false),
            Player(blackName, Color::BLACK, false)
        );
        ConsoleUI::displayMessage("New game started!");
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleSave(const std::string& filename) {
    try {
        if (filename.empty()) {
            ConsoleUI::displayError("Please specify filename");
            return;
        }
        gameController.saveGame(filename);
        ConsoleUI::displayGameSaved(filename);
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleLoad(const std::string& filename) {
    try {
        if (filename.empty()) {
            ConsoleUI::displayError("Please specify filename");
            return;
        }
        gameController.loadGame(filename);
        ConsoleUI::displayGameLoaded(filename);
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleResign() {
    try {
        const Game& game = gameController.getGame();
        gameController.resign(game.getCurrentPlayerColor());
        ConsoleUI::displayMessage("You resigned. Game over.");
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleDrawOffer() {
    try {
        gameController.offerDraw();
        ConsoleUI::displayMessage("Draw offered");
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleDrawAccept() {
    try {
        gameController.acceptDraw();
        ConsoleUI::displayMessage("Draw accepted. Game over.");
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleUndo() {
    try {
        gameController.undo();
        ConsoleUI::displayMessage("Move undone");
    } catch (const std::exception& e) {
        ConsoleUI::displayError(e.what());
    }
}

void CommandHandler::handleHelp() {
    ConsoleUI::displayHelp();
}

}
