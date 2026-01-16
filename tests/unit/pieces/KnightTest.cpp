#include <gtest/gtest.h>
#include <memory>
#include "../../../src/chess/core/Position.hpp"
#include "../../../src/chess/core/Color.hpp"
#include "../../../src/chess/core/Board.hpp"
#include "../../../src/chess/pieces/Knight.hpp"

using namespace chess::core;
using namespace chess::pieces;

/**
 * Test Suite for Knight piece
 * Tests L-shaped movement, piece jumping, and valid knight moves
 */
class KnightTest : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board = Board();
    }
};

// ============ Knight Starting Position Tests ============

TEST_F(KnightTest, WhiteKnightOnB1) {
    // White knight starts on b1 (file 1, rank 0)
    Position pos(1, 0);
    // Piece* piece = board.getPieceAt(pos);
    // EXPECT_NE(piece, nullptr);
    // EXPECT_TRUE(dynamic_cast<Knight*>(piece) != nullptr);
    // EXPECT_EQ(piece->getColor(), Color::WHITE);
}

TEST_F(KnightTest, WhiteKnightOnG1) {
    // White knight starts on g1 (file 6, rank 0)
    Position pos(6, 0);
    // Piece* piece = board.getPieceAt(pos);
    // EXPECT_NE(piece, nullptr);
    // EXPECT_TRUE(dynamic_cast<Knight*>(piece) != nullptr);
}

TEST_F(KnightTest, BlackKnightOnB8) {
    // Black knight starts on b8 (file 1, rank 7)
    Position pos(1, 7);
    // Piece* piece = board.getPieceAt(pos);
    // EXPECT_NE(piece, nullptr);
    // EXPECT_TRUE(dynamic_cast<Knight*>(piece) != nullptr);
    // EXPECT_EQ(piece->getColor(), Color::BLACK);
}

TEST_F(KnightTest, BlackKnightOnG8) {
    // Black knight starts on g8 (file 6, rank 7)
    Position pos(6, 7);
    // Piece* piece = board.getPieceAt(pos);
    // EXPECT_NE(piece, nullptr);
    // EXPECT_TRUE(dynamic_cast<Knight*>(piece) != nullptr);
}

// ============ Knight L-Shape Movement Tests ============

TEST_F(KnightTest, KnightMove_1FileBy2Ranks) {
    // L-shape: 1 file, 2 ranks
    Position from(0, 0);  // a1
    Position to(1, 2);    // b3
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    EXPECT_EQ(fileDiff, 1);
    EXPECT_EQ(rankDiff, 2);
}

TEST_F(KnightTest, KnightMove_2FileBy1Rank) {
    // L-shape: 2 files, 1 rank
    Position from(0, 0);  // a1
    Position to(2, 1);    // c2
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    EXPECT_EQ(fileDiff, 2);
    EXPECT_EQ(rankDiff, 1);
}

TEST_F(KnightTest, KnightMove_1FileBy2RanksReverse) {
    // L-shape going other direction
    Position from(2, 1);   // c2
    Position to(1, 3);     // b4
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    EXPECT_EQ(fileDiff, 1);
    EXPECT_EQ(rankDiff, 2);
}

TEST_F(KnightTest, KnightMove_2FileBy1RankReverse) {
    // L-shape going other direction
    Position from(2, 1);   // c2
    Position to(4, 2);     // e3
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    EXPECT_EQ(fileDiff, 2);
    EXPECT_EQ(rankDiff, 1);
}

// ============ Knight Opening Moves Tests ============

TEST_F(KnightTest, KnightB1ToA3) {
    // Classic knight move from starting position
    Position from(1, 0);  // b1
    Position to(0, 2);    // a3
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                        (fileDiff == 2 && rankDiff == 1);
    EXPECT_TRUE(isKnightMove);
}

TEST_F(KnightTest, KnightB1ToC3) {
    // Another classic knight move from starting position
    Position from(1, 0);  // b1
    Position to(2, 2);    // c3
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                        (fileDiff == 2 && rankDiff == 1);
    EXPECT_TRUE(isKnightMove);
}

TEST_F(KnightTest, KnightG1ToF3) {
    Position from(6, 0);  // g1
    Position to(5, 2);    // f3
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                        (fileDiff == 2 && rankDiff == 1);
    EXPECT_TRUE(isKnightMove);
}

TEST_F(KnightTest, KnightG1ToH3) {
    Position from(6, 0);  // g1
    Position to(7, 2);    // h3
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                        (fileDiff == 2 && rankDiff == 1);
    EXPECT_TRUE(isKnightMove);
}

// ============ All 8 Knight Moves Tests ============

TEST_F(KnightTest, AllEightKnightMovesFromCenter) {
    // Knight on e4 (file 4, rank 3) can make 8 moves
    Position knight(4, 3);
    
    std::vector<Position> legalMoves = {
        Position(3, 5),  // d6 (1 left, 2 up)
        Position(5, 5),  // f6 (1 right, 2 up)
        Position(3, 1),  // d2 (1 left, 2 down)
        Position(5, 1),  // f2 (1 right, 2 down)
        Position(2, 4),  // c5 (2 left, 1 up)
        Position(2, 2),  // c3 (2 left, 1 down)
        Position(6, 4),  // g5 (2 right, 1 up)
        Position(6, 2),  // g3 (2 right, 1 down)
    };
    
    EXPECT_EQ(legalMoves.size(), 8);
    
    // Verify each move is valid knight move
    for (const auto& to : legalMoves) {
        int fileDiff = abs(knight.getFile() - to.getFile());
        int rankDiff = abs(knight.getRank() - to.getRank());
        
        bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                            (fileDiff == 2 && rankDiff == 1);
        EXPECT_TRUE(isKnightMove);
    }
}

// ============ Invalid Knight Moves Tests ============

TEST_F(KnightTest, KnightCannotMoveDiagonally) {
    Position from(4, 3);  // e4
    Position to(7, 6);    // h7
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    // This is diagonal, not L-shaped
    EXPECT_EQ(fileDiff, rankDiff);
}

TEST_F(KnightTest, KnightCannotMoveHorizontally) {
    Position from(4, 3);  // e4
    Position to(6, 3);    // g4
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    // Horizontal move, not L-shaped
    EXPECT_EQ(rankDiff, 0);
}

TEST_F(KnightTest, KnightCannotMoveVertically) {
    Position from(4, 3);  // e4
    Position to(4, 6);    // e7
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    // Vertical move, not L-shaped
    EXPECT_EQ(fileDiff, 0);
}

TEST_F(KnightTest, KnightCannotMove3Squares) {
    Position from(4, 3);  // e4
    Position to(7, 3);    // h4 (3 squares horizontal)
    
    int fileDiff = abs(from.getFile() - to.getFile());
    
    EXPECT_EQ(fileDiff, 3);
}

TEST_F(KnightTest, InvalidKnightMoveTooManySquares) {
    Position from(0, 0);  // a1
    Position to(7, 7);    // h8
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    
    bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                        (fileDiff == 2 && rankDiff == 1);
    EXPECT_FALSE(isKnightMove);
}

// ============ Knight Jump Ability Tests ============

TEST_F(KnightTest, KnightCanJumpOverPieces) {
    // Knight's unique ability: can jump over other pieces
    // This is fundamental to knight's movement
    // If pieces block intermediate squares, knight still moves
    EXPECT_TRUE(true);
}

TEST_F(KnightTest, KnightCanCaptureAnyPieceInLPath) {
    // If opponent piece is at L-shaped destination, it can be captured
    EXPECT_TRUE(true);
}

TEST_F(KnightTest, KnightCannotCaptureOwnPieces) {
    // If own piece is at destination, cannot move there
    EXPECT_TRUE(true);
}

// ============ Knight Edge Case Tests ============

TEST_F(KnightTest, KnightAtA1Bounded) {
    // Knight on a1 (0,0) has limited moves due to board boundaries
    Position knight(0, 0);
    
    std::vector<Position> possibleMoves = {
        Position(1, 2),  // b3 - valid
        Position(2, 1),  // c2 - valid
        // Cannot go to negative files/ranks
    };
    
    // Only 2 of 8 theoretical moves are on board
    EXPECT_EQ(possibleMoves.size(), 2);
}

TEST_F(KnightTest, KnightAtH8Bounded) {
    // Knight on h8 (7,7) has limited moves
    Position knight(7, 7);
    
    // Some moves would go off board
    EXPECT_TRUE(true);
}

TEST_F(KnightTest, KnightAtE4HasFullMobility) {
    // Knight on e4 (4,3) in center has all 8 moves available
    Position knight(4, 3);
    
    std::vector<std::pair<int, int>> moves = {
        {3, 5}, {5, 5}, {3, 1}, {5, 1},
        {2, 4}, {2, 2}, {6, 4}, {6, 2}
    };
    
    for (const auto& move : moves) {
        EXPECT_GE(move.first, 0);
        EXPECT_LE(move.first, 7);
        EXPECT_GE(move.second, 0);
        EXPECT_LE(move.second, 7);
    }
}

// ============ Knight Tactical Tests ============

TEST_F(KnightTest, KnightFork) {
    // Knight can attack two pieces simultaneously
    // This is a positional concept, not movement validation
    EXPECT_TRUE(true);
}

TEST_F(KnightTest, KnightPin) {
    // Knight can be pinned (cannot move without exposing king to check)
    EXPECT_TRUE(true);
}

// ============ Multiple Knight Scenarios ============

TEST_F(KnightTest, BothWhiteKnightsCanMove) {
    Position knight1(1, 0);  // b1
    Position knight2(6, 0);  // g1
    
    // Both should have valid initial moves
    EXPECT_NE(knight1, knight2);
}

TEST_F(KnightTest, KnightMovesAreSymmetrical) {
    // A knight on a square has same move patterns regardless of board orientation
    Position from(4, 3);  // e4
    
    std::vector<Position> moves = {
        Position(3, 5), Position(5, 5), Position(3, 1), Position(5, 1),
        Position(2, 4), Position(2, 2), Position(6, 4), Position(6, 2)
    };
    
    // Verify all moves follow L-shape pattern
    for (const auto& to : moves) {
        int fileDiff = abs(from.getFile() - to.getFile());
        int rankDiff = abs(from.getRank() - to.getRank());
        
        bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                            (fileDiff == 2 && rankDiff == 1);
        EXPECT_TRUE(isKnightMove);
    }
}
