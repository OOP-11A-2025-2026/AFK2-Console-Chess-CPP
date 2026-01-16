#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <thread>

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

using namespace chess::core;
using namespace chess::pieces;
using namespace chess::rules;
using namespace chess::util;

// Color codes for terminal output
const char* GREEN = "\033[32m";
const char* RED = "\033[31m";
const char* RESET = "\033[0m";

void printTest(const std::string& name, bool passed) {
    if (passed) {
        std::cout << GREEN << "✓ PASS" << RESET << " - " << name << std::endl;
    } else {
        std::cout << RED << "✗ FAIL" << RESET << " - " << name << std::endl;
    }
}

void testColor() {
    std::cout << "\n=== Testing Color ===" << std::endl;

    Color white = Color::WHITE;
    Color black = Color::BLACK;

    // Test opposite
    printTest("Color::opposite(WHITE) == BLACK", opposite(white) == Color::BLACK);
    printTest("Color::opposite(BLACK) == WHITE", opposite(black) == Color::WHITE);
}

void testPosition() {
    std::cout << "\n=== Testing Position ===" << std::endl;

    // Test valid position creation
    try {
        Position p1(4, 4);
        printTest("Create Position(4, 4)", true);
    } catch (...) {
        printTest("Create Position(4, 4)", false);
    }

    // Test invalid position throws
    bool exceptionThrown = false;
    try {
        Position p2(8, 8);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    printTest("Position(8, 8) throws std::invalid_argument", exceptionThrown);

    // Test algebraic notation
    Position e4(4, 3);
    printTest("Position(4, 3).toAlgebraic() == 'e4'", e4.toAlgebraic() == "e4");

    // Test parsing algebraic notation
    Position e4_parsed = Position::fromAlgebraic("e4");
    printTest("Position::fromAlgebraic('e4') == Position(4, 3)", 
              e4_parsed == Position(4, 3));

    // Test equality
    Position p1(0, 0);
    Position p2(0, 0);
    printTest("Position(0, 0) == Position(0, 0)", p1 == p2);
}

void testMove() {
    std::cout << "\n=== Testing Move ===" << std::endl;

    Board board;
    Position e2(4, 1);
    Position e4(4, 3);

    Piece* pawn = board.getPiece(e2);
    assert(pawn != nullptr);

    try {
        Move::Builder builder(e2, e4, pawn);
        Move move = builder.build();
        printTest("Create Move e2->e4", true);
        printTest("Move.getFrom() == e2", move.getFrom() == e2);
        printTest("Move.getTo() == e4", move.getTo() == e4);
    } catch (...) {
        printTest("Create Move e2->e4", false);
    }
}

void testBoard() {
    std::cout << "\n=== Testing Board ===" << std::endl;

    Board board;

    // Test initial setup
    Piece* e2_pawn = board.getPiece(Position(4, 1));
    printTest("White pawn at e2", e2_pawn != nullptr && 
              e2_pawn->getColor() == Color::WHITE &&
              e2_pawn->getPieceName() == "Pawn");

    Piece* e1_empty = board.getPiece(Position(4, 2));
    printTest("e3 is empty", e1_empty == nullptr);

    // Test king positions
    Position whiteKingPos = board.getKingPosition(Color::WHITE);
    printTest("White king at e1", whiteKingPos == Position(4, 0));

    Position blackKingPos = board.getKingPosition(Color::BLACK);
    printTest("Black king at e8", blackKingPos == Position(4, 7));

    // Test move piece
    try {
        Position from(4, 1);
        Position to(4, 3);
        auto captured = board.movePiece(from, to);
        printTest("Move pawn e2->e4", captured == nullptr);
        printTest("e4 has pawn", board.getPiece(to) != nullptr);
        printTest("e2 is empty", board.getPiece(from) == nullptr);
    } catch (...) {
        printTest("Move pawn e2->e4", false);
    }

    // Test deep copy
    Board boardCopy = board;
    printTest("Board deep copy", 
              boardCopy.getPiece(Position(4, 3)) != nullptr &&
              boardCopy.getPiece(Position(4, 1)) == nullptr);
}

void testPieces() {
    std::cout << "\n=== Testing Pieces ===" << std::endl;

    Board board;

    // Test pawn destinations
    Piece* pawn = board.getPiece(Position(4, 1));
    auto pawnDests = pawn->getLegalDestinations(board);
    printTest("Pawn at e2 has legal moves", pawnDests.size() == 2);
    printTest("Pawn can move to e3", 
              std::find(pawnDests.begin(), pawnDests.end(), Position(4, 2)) != pawnDests.end());
    printTest("Pawn can move to e4", 
              std::find(pawnDests.begin(), pawnDests.end(), Position(4, 3)) != pawnDests.end());

    // Test knight destinations
    Piece* knight = board.getPiece(Position(1, 0));
    auto knightDests = knight->getLegalDestinations(board);
    printTest("Knight at b1 has 2 legal moves", knightDests.size() == 2);

    // Test rook is blocked
    Piece* rook = board.getPiece(Position(0, 0));
    auto rookDests = rook->getLegalDestinations(board);
    printTest("Rook at a1 has no legal moves (blocked)", rookDests.size() == 0);

    // Test piece symbols
    Piece* king = board.getPiece(Position(4, 0));
    printTest("White king symbol is K", king->getSymbol() == 'K');

    Piece* queen = board.getPiece(Position(3, 0));
    printTest("White queen symbol is Q", queen->getSymbol() == 'Q');
}

void testPlayer() {
    std::cout << "\n=== Testing Player ===" << std::endl;

    try {
        Player p1("Alice", Color::WHITE, false);
        printTest("Create Player 'Alice' (WHITE)", true);
        printTest("Player name is 'Alice'", p1.getName() == "Alice");
        printTest("Player color is WHITE", p1.getColor() == Color::WHITE);
        printTest("Player is not bot", !p1.isBot_());
    } catch (...) {
        printTest("Create Player 'Alice' (WHITE)", false);
    }

    // Test empty name throws
    bool exceptionThrown = false;
    try {
        Player p2("", Color::BLACK);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    printTest("Player with empty name throws", exceptionThrown);
}

void testChessClock() {
    std::cout << "\n=== Testing ChessClock ===" << std::endl;

    try {
        ChessClock clock(60000);  // 60 seconds
        printTest("Create ChessClock with 60 seconds", true);

        long remaining = clock.getRemainingTime(Color::WHITE);
        printTest("White has ~60 seconds", remaining <= 60000 && remaining >= 59990);

        clock.startTurn(Color::WHITE);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        clock.stopTurn();
        long remaining2 = clock.getRemainingTime(Color::WHITE);
        printTest("After stopTurn, time decreased", remaining2 < remaining);

        printTest("isFlagFallen returns false", !clock.isFlagFallen(Color::WHITE));
    } catch (...) {
        printTest("ChessClock operations", false);
    }

    // Test invalid clock throws
    bool exceptionThrown = false;
    try {
        ChessClock badClock(-1000);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    printTest("ChessClock with negative time throws", exceptionThrown);
}

void testGame() {
    std::cout << "\n=== Testing Game ===" << std::endl;

    try {
        auto white = std::make_unique<Player>("Alice", Color::WHITE);
        auto black = std::make_unique<Player>("Bob", Color::BLACK);
        auto clock = std::make_unique<ChessClock>(5 * 60 * 1000);

        Game game(std::move(white), std::move(black), std::move(clock));
        printTest("Create Game", true);

        printTest("Current player is WHITE", game.getCurrentPlayerColor() == Color::WHITE);
        printTest("Game state is ONGOING", game.getGameState() == GameState::ONGOING);
        printTest("Move history is empty", game.getMoveHistory().size() == 0);
    } catch (...) {
        printTest("Create Game", false);
    }
}

void testMoveValidator() {
    std::cout << "\n=== Testing MoveValidator ===" << std::endl;

    Board board;

    // Test valid pawn move
    Piece* pawn = board.getPiece(Position(4, 1));
    Move move1 = Move::Builder(Position(4, 1), Position(4, 3), pawn).build();
    bool valid1 = MoveValidator::isValidMove(board, move1, Color::WHITE);
    printTest("Pawn e2->e4 is valid", valid1);

    // Test invalid pawn move (backward)
    Move move2 = Move::Builder(Position(4, 3), Position(4, 1), pawn).build();
    bool valid2 = MoveValidator::isValidMove(board, move2, Color::WHITE);
    printTest("Pawn e4->e2 is invalid", !valid2);

    // Test moving opponent's piece
    Piece* blackPawn = board.getPiece(Position(4, 6));
    Move move3 = Move::Builder(Position(4, 6), Position(4, 5), blackPawn).build();
    bool valid3 = MoveValidator::isValidMove(board, move3, Color::WHITE);
    printTest("Cannot move opponent's piece", !valid3);
}

void testAlgebraicNotation() {
    std::cout << "\n=== Testing Algebraic Notation ===" << std::endl;

    Board board;
    Piece* pawn = board.getPiece(Position(4, 1));

    Move move = Move::Builder(Position(4, 1), Position(4, 3), pawn).build();

    std::string longAlg = AlgebraicNotationUtil::toLongAlgebraic(move);
    printTest("toLongAlgebraic e2->e4 == 'e2e4'", longAlg == "e2e4");

    // Test parsing
    auto parsed = AlgebraicNotationUtil::parseMove("e2e4", board, Color::WHITE);
    printTest("parseMove 'e2e4' succeeds", parsed != nullptr);

    // Test parsing with space
    auto parsed2 = AlgebraicNotationUtil::parseMove("e2 e4", board, Color::WHITE);
    printTest("parseMove 'e2 e4' succeeds", parsed2 != nullptr);
}

int main() {
    std::cout << "======================================" << std::endl;
    std::cout << "     AFK2 C++ Chess Engine Tests      " << std::endl;
    std::cout << "======================================" << std::endl;

    testColor();
    testPosition();
    testMove();
    testBoard();
    testPieces();
    testPlayer();
    testChessClock();
    testGame();
    testMoveValidator();
    testAlgebraicNotation();

    std::cout << "\n======================================" << std::endl;
    std::cout << "          Tests Completed             " << std::endl;
    std::cout << "======================================" << std::endl;

    return 0;
}
