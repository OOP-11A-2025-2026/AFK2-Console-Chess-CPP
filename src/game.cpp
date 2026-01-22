#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <thread>
#include <sstream>
#include <fstream>

// Core includes
#include "chess/core/Color.hpp"
#include "chess/core/Position.hpp"
#include "chess/core/Move.hpp"
#include "chess/core/Piece.hpp"
#include "chess/core/Board.hpp"
#include "chess/core/Player.hpp"
#include "chess/core/Game.hpp"
#include "chess/core/ChessClock.hpp"
#include "chess/core/GameState.hpp"

// Piece includes
#include "chess/pieces/Pawn.hpp"
#include "chess/pieces/Knight.hpp"
#include "chess/pieces/Bishop.hpp"
#include "chess/pieces/Rook.hpp"
#include "chess/pieces/Queen.hpp"
#include "chess/pieces/King.hpp"

// Rules and utilities
#include "chess/rules/MoveValidator.hpp"
#include "chess/util/AlgebraicNotationUtil.hpp"
#include "chess/pgn/PgnIO.hpp"
#include "chess/io/ConsoleUI.hpp"

using namespace chess::core;
using namespace chess::pieces;
using namespace chess::rules;
using namespace chess::util;
using namespace chess::io;

// Color codes for terminal output
const char* GREEN = "\033[32m";
const char* RED = "\033[31m";
const char* YELLOW = "\033[33m";
const char* RESET = "\033[0m";

// ============================================================================
// INTERACTIVE GAME MODE
// ============================================================================

void playInteractiveGame() {
    std::cout << "\n" << YELLOW << "=== AFK2 Console Chess ===" << RESET << "\n\n";
    
    std::string whiteName, blackName;
    std::cout << "White player name: ";
    std::getline(std::cin, whiteName);
    if (whiteName.empty()) whiteName = "White";
    
    std::cout << "Black player name: ";
    std::getline(std::cin, blackName);
    if (blackName.empty()) blackName = "Black";
    
    auto white = std::make_unique<Player>(whiteName, Color::WHITE);
    auto black = std::make_unique<Player>(blackName, Color::BLACK);
    auto clock = std::make_unique<ChessClock>(5 * 60 * 1000);  // 5 minutes
    
    Game game(std::move(white), std::move(black), std::move(clock));
    
    std::cout << "\n" << GREEN << "Game started!" << RESET << "\n";
    std::cout << "Commands: move (e2e4), save <file>, load <file>, undo, resign, draw, help, exit\n\n";
    
    while (game.getGameState() == GameState::ONGOING) {
        ConsoleUI::clearScreen();
        ConsoleUI::displayBoard(game.getBoard());
        ConsoleUI::displayGameStatus(game);
        
        const Player& current = game.getCurrentPlayer();
        std::cout << "\n" << YELLOW;
        ConsoleUI::displayPlayerTurnWithTime(current, game);
        std::cout << RESET;
        ConsoleUI::displayPromptForInput();
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) continue;
        
        // Parse command
        std::istringstream iss(input);
        std::string cmd;
        iss >> cmd;
        
        if (cmd == "help") {
            std::cout << "\nCommands:\n"
                     << "  <move>      - Play a move (e.g., e2e4, e2 e4)\n"
                     << "  save <file> - Save game to PGN file\n"
                     << "  load <file> - Load game from PGN file\n"
                     << "  undo        - Undo last move\n"
                     << "  resign      - Resign the game\n"
                     << "  draw        - Offer/accept draw\n"
                     << "  exit        - Quit without saving\n";
            continue;
        }
        
        if (cmd == "exit") {
            std::cout << "Thanks for playing!\n";
            return;
        }
        
        if (cmd == "save") {
            std::string filename;
            iss >> filename;
            if (filename.empty()) {
                std::cout << RED << "Please specify a filename\n" << RESET;
                continue;
            }
            try {
                chess::pgn::PgnIO::saveToFile(filename, game);
                std::cout << GREEN << "Game saved to " << filename << RESET << "\n";
            } catch (const std::exception& e) {
                std::cout << RED << "Error saving: " << e.what() << RESET << "\n";
            }
            continue;
        }
        
        if (cmd == "load") {
            std::string filename;
            iss >> filename;
            if (filename.empty()) {
                std::cout << RED << "Please specify a filename\n" << RESET;
                continue;
            }
            try {
                game.reset();
                auto result = chess::pgn::PgnIO::loadFromFile(filename, game);
                std::cout << GREEN << "Game loaded from " << filename << RESET << "\n";
            } catch (const std::exception& e) {
                std::cout << RED << "Error loading: " << e.what() << RESET << "\n";
            }
            continue;
        }
        
        if (cmd == "undo") {
            std::cout << YELLOW << "Undo not yet implemented\n" << RESET;
            continue;
        }
        
        if (cmd == "resign") {
            game.resign(game.getCurrentPlayerColor());
            std::cout << GREEN << current.getName() << " resigns. Game over!" << RESET << "\n";
            continue;
        }
        
        if (cmd == "draw") {
            if (game.isDrawOfferPending()) {
                game.acceptDraw();
                std::cout << GREEN << "Draw accepted. Game over!" << RESET << "\n";
            } else {
                game.offerDraw(game.getCurrentPlayerColor());
                std::cout << YELLOW << current.getName() << " offers a draw\n" << RESET;
            }
            continue;
        }
        
        // Try to parse as a move
        try {
            auto movePtr = AlgebraicNotationUtil::parseMove(input, game.getBoard(), 
                                                            game.getCurrentPlayerColor());
            if (!movePtr) {
                std::cout << RED << "Invalid move notation\n" << RESET;
                continue;
            }
            
            if (!MoveValidator::isValidMove(game.getBoard(), *movePtr, 
                                           game.getCurrentPlayerColor())) {
                std::cout << RED << "Illegal move\n" << RESET;
                continue;
            }
            
            // Apply the move
            game.getBoard().movePiece(movePtr->getFrom(), movePtr->getTo());
            game.addMove(*movePtr);
            game.switchTurn();
            game.updateState();
            
            std::cout << GREEN << "Move: " << AlgebraicNotationUtil::toLongAlgebraic(*movePtr) 
                     << RESET << "\n";
        } catch (const std::exception& e) {
            std::cout << RED << "Error: " << e.what() << RESET << "\n";
        }
    }
    
    // Game ended
    ConsoleUI::clearScreen();
    ConsoleUI::displayBoard(game.getBoard());
    ConsoleUI::displayGameStatus(game);
    std::cout << "\n" << GREEN << "Game Over!\n" << RESET;
    if (game.getGameState() == GameState::RESIGNATION) {
        std::cout << "Opponent resigned.\n";
    } else if (game.getGameState() == GameState::DRAW_BY_AGREEMENT) {
        std::cout << "Game drawn by agreement.\n";
    }
}

int main() {
    std::cout << "\n" << YELLOW << "╔════════════════════════════════╗\n"
             << "║   AFK2 Console Chess (C++)      ║\n"
             << "║   FIDE Rules Compliant           ║\n"
             << "╚════════════════════════════════╝" << RESET << "\n";
    
    playInteractiveGame();
    
    return 0;
}
