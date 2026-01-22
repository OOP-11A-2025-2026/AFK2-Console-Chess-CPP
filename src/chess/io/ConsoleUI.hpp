#ifndef CHESS_IO_CONSOLEUI_HPP
#define CHESS_IO_CONSOLEUI_HPP

#include "../core/Game.hpp"
#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include <string>
#include <iostream>

namespace chess::io {

class ConsoleUI {
public:
    static void displayWelcome();

    static void displayHelp();

    static void displayBoard(const chess::core::Board& board);

    static void displayGameStatus(const chess::core::Game& game);

    static void displayMove(const chess::core::Move& move);

    static void displayCheck(chess::core::Color color);

    static void displayCheckmate(chess::core::Color winner);

    static void displayStalemate();

    static void displayDraw(const std::string& reason);

    static void displayInvalidMove(const std::string& reason);

    static void displayMessage(const std::string& message);

    static void displayError(const std::string& error);

    static void displayPlayerTurn(const chess::core::Player& player);

    static void displayPlayerTurnWithTime(const chess::core::Player& player, const chess::core::Game& game);

    static void displayPromptForInput();

    static void clearScreen();

    static void displayGameSaved(const std::string& filename);

    static void displayGameLoaded(const std::string& filename);

private:
    static std::string pieceToSymbol(chess::core::Piece* piece);

    static std::string colorToString(chess::core::Color color);

    static std::string positionToAlgebraic(const chess::core::Position& pos);
};

}

#endif
