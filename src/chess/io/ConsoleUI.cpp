#include "ConsoleUI.hpp"
#include "../core/Color.hpp"
#include "../core/Piece.hpp"
#include "../core/GameState.hpp"
#include "../pieces/Pawn.hpp"
#include "../pieces/Rook.hpp"
#include "../pieces/Knight.hpp"
#include "../pieces/Bishop.hpp"
#include "../pieces/Queen.hpp"
#include "../pieces/King.hpp"
#include <iostream>
#include <iomanip>

namespace chess::io {

using namespace chess::core;

std::string ConsoleUI::pieceToSymbol(Piece* piece) {
    if (!piece) return " ";

    char symbol;
    if (dynamic_cast<pieces::Pawn*>(piece)) symbol = 'P';
    else if (dynamic_cast<pieces::Rook*>(piece)) symbol = 'R';
    else if (dynamic_cast<pieces::Knight*>(piece)) symbol = 'N';
    else if (dynamic_cast<pieces::Bishop*>(piece)) symbol = 'B';
    else if (dynamic_cast<pieces::Queen*>(piece)) symbol = 'Q';
    else if (dynamic_cast<pieces::King*>(piece)) symbol = 'K';
    else return " ";

    if (piece->getColor() == Color::WHITE) {
        return std::string(1, symbol);
    } else {
        return std::string(1, std::tolower(symbol));
    }
}

std::string ConsoleUI::colorToString(Color color) {
    return (color == Color::WHITE) ? "White" : "Black";
}

std::string ConsoleUI::positionToAlgebraic(const Position& pos) {
    std::string result;
    result += (char)('a' + pos.getFile());
    result += (char)('1' + pos.getRank());
    return result;
}

void ConsoleUI::displayWelcome() {
    std::cout << "\n";
    std::cout << "================================\n";
    std::cout << "    CHESS GAME IN CONSOLE\n";
    std::cout << "================================\n";
    std::cout << "Type 'help' for commands\n";
    std::cout << "================================\n\n";
}

void ConsoleUI::displayHelp() {
    std::cout << "\n=== COMMANDS ===\n";
    std::cout << "Move notation:\n";
    std::cout << "  e2 e4      (coordinate: from to)\n";
    std::cout << "  Nf3        (algebraic notation)\n";
    std::cout << "Game commands:\n";
    std::cout << "  new        (start new game)\n";
    std::cout << "  save file  (save game to file)\n";
    std::cout << "  load file  (load game from file)\n";
    std::cout << "  undo       (undo last move)\n";
    std::cout << "  resign     (resign game)\n";
    std::cout << "  draw       (offer draw)\n";
    std::cout << "  accept     (accept draw)\n";
    std::cout << "  help       (show this)\n";
    std::cout << "  exit       (exit game)\n";
    std::cout << "================\n\n";
}

void ConsoleUI::displayBoard(const Board& board) {
    std::cout << "\n  a b c d e f g h\n";
    std::cout << "  _______________\n";

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << " |";
        for (int file = 0; file < 8; ++file) {
            Position pos(file, rank);
            Piece* piece = board.getPiece(pos);
            std::cout << pieceToSymbol(piece) << "|";
        }
        std::cout << " " << (rank + 1) << "\n";
    }

    std::cout << "  _______________\n";
    std::cout << "  a b c d e f g h\n\n";
}

void ConsoleUI::displayGameStatus(const Game& game) {
    std::cout << "White: " << game.getWhitePlayer().getName() << " vs ";
    std::cout << "Black: " << game.getBlackPlayer().getName() << "\n";
    std::cout << "Current: " << colorToString(game.getCurrentPlayerColor());

    GameState state = game.getGameState();
    if (state == GameState::CHECK) {
        std::cout << " (CHECK)";
    }
    std::cout << "\n\n";
}

void ConsoleUI::displayMove(const Move& move) {
    std::cout << positionToAlgebraic(move.getFrom()) << " -> "
              << positionToAlgebraic(move.getTo()) << "\n";
}

void ConsoleUI::displayCheck(Color color) {
    std::cout << colorToString(color) << " is in CHECK!\n";
}

void ConsoleUI::displayCheckmate(Color winner) {
    std::cout << colorToString(opposite(winner)) << " is CHECKMATE!\n";
    std::cout << colorToString(winner) << " wins!\n";
}

void ConsoleUI::displayStalemate() {
    std::cout << "STALEMATE! Game is drawn.\n";
}

void ConsoleUI::displayDraw(const std::string& reason) {
    std::cout << "DRAW by " << reason << "!\n";
}

void ConsoleUI::displayInvalidMove(const std::string& reason) {
    std::cout << "Invalid move: " << reason << "\n";
}

void ConsoleUI::displayMessage(const std::string& message) {
    std::cout << message << "\n";
}

void ConsoleUI::displayError(const std::string& error) {
    std::cout << "ERROR: " << error << "\n";
}

void ConsoleUI::displayPlayerTurn(const Player& player) {
    std::cout << player.getName() << "'s turn\n";
}

void ConsoleUI::displayPromptForInput() {
    std::cout << "> ";
}

void ConsoleUI::clearScreen() {
    std::system("clear");
}

void ConsoleUI::displayGameSaved(const std::string& filename) {
    std::cout << "Game saved to " << filename << "\n";
}

void ConsoleUI::displayGameLoaded(const std::string& filename) {
    std::cout << "Game loaded from " << filename << "\n";
}

}
