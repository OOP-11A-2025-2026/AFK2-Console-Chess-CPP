#include "ConsoleUI.hpp"
#include "../core/Color.hpp"
#include "../core/Piece.hpp"
#include "../core/GameState.hpp"
#include "../core/ChessClock.hpp"
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

    // Unicode chess piece symbols
    if (piece->getColor() == Color::WHITE) {
        // White pieces
        if (dynamic_cast<pieces::King*>(piece)) return "♔";
        else if (dynamic_cast<pieces::Queen*>(piece)) return "♕";
        else if (dynamic_cast<pieces::Rook*>(piece)) return "♖";
        else if (dynamic_cast<pieces::Bishop*>(piece)) return "♗";
        else if (dynamic_cast<pieces::Knight*>(piece)) return "♘";
        else if (dynamic_cast<pieces::Pawn*>(piece)) return "♙";
    } else {
        // Black pieces
        if (dynamic_cast<pieces::King*>(piece)) return "♚";
        else if (dynamic_cast<pieces::Queen*>(piece)) return "♛";
        else if (dynamic_cast<pieces::Rook*>(piece)) return "♜";
        else if (dynamic_cast<pieces::Bishop*>(piece)) return "♝";
        else if (dynamic_cast<pieces::Knight*>(piece)) return "♞";
        else if (dynamic_cast<pieces::Pawn*>(piece)) return "♟";
    }
    return " ";
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
    // ANSI color codes
    const std::string RESET = "\033[0m";
    const std::string WHITE_BG = "\033[47m";  // White background
    const std::string BLACK_BG = "\033[40m";  // Black background
    const std::string WHITE_TEXT = "\033[37m"; // White text
    const std::string BLACK_TEXT = "\033[30m"; // Black text

    std::cout << "\n  ";
    for (char file = 'a'; file <= 'h'; ++file) {
        std::cout << " " << file << " ";
    }
    std::cout << "\n";

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << (rank + 1) << " ";
        for (int file = 0; file < 8; ++file) {
            Position pos(file, rank);
            Piece* piece = board.getPiece(pos);
            
            // Determine square color (a1 is white, alternates)
            bool isWhiteSquare = (file + rank) % 2 == 0;
            
            // Set background color
            if (isWhiteSquare) {
                std::cout << WHITE_BG << BLACK_TEXT;
            } else {
                std::cout << BLACK_BG << WHITE_TEXT;
            }
            
            // Display piece or empty space
            std::cout << " " << pieceToSymbol(piece) << " ";
            std::cout << RESET;
        }
        std::cout << " " << (rank + 1) << "\n";
    }

    std::cout << "  ";
    for (char file = 'a'; file <= 'h'; ++file) {
        std::cout << " " << file << " ";
    }
    std::cout << "\n\n";
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

void ConsoleUI::displayPlayerTurnWithTime(const Player& player, const Game& game) {
    const ChessClock& clock = game.getClock();
    std::string timeStr = clock.getFormattedTime(player.getColor());
    std::cout << player.getName() << "'s turn - Time: " << timeStr << "\n";
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
