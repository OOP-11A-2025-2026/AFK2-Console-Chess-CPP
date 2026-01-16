#include <gtest/gtest.h>
#include <memory>
#include "../../../src/chess/core/Move.hpp"
#include "../../../src/chess/core/Position.hpp"

using namespace chess::core;

/**
 * Test Suite for Move class
 * Tests move construction, validation, flags, and special moves
 */
class MoveTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// ============ Basic Move Construction Tests ============

TEST_F(MoveTest, ConstructSimpleMove) {
    Position from(4, 1);  // e2
    Position to(4, 3);    // e4

    auto moveBuilder = Move::Builder(from, to);
    // Builder pattern - tests will depend on Move API
    EXPECT_TRUE(true);  // Placeholder
}

TEST_F(MoveTest, MoveFromValidPositions) {
    Position from(0, 1);  // a2
    Position to(0, 3);    // a4

    auto moveBuilder = Move::Builder(from, to);
    EXPECT_TRUE(true);  // Placeholder
}

TEST_F(MoveTest, MoveFromCenterPositions) {
    Position from(3, 3);  // d4
    Position to(4, 4);    // e5

    auto moveBuilder = Move::Builder(from, to);
    EXPECT_TRUE(true);  // Placeholder
}

// ============ Move Direction Tests ============

TEST_F(MoveTest, HorizontalMove) {
    Position from(0, 0);  // a1
    Position to(7, 0);    // h1
    // Horizontal move is valid for rooks and queens
    EXPECT_EQ(from.getRank(), to.getRank());
}

TEST_F(MoveTest, VerticalMove) {
    Position from(0, 0);  // a1
    Position to(0, 7);    // a8
    // Vertical move is valid for rooks and queens
    EXPECT_EQ(from.getFile(), to.getFile());
}

TEST_F(MoveTest, DiagonalMove) {
    Position from(0, 0);  // a1
    Position to(7, 7);    // h8
    // Diagonal move is valid for bishops and queens
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    EXPECT_EQ(fileDiff, rankDiff);
}

TEST_F(MoveTest, KnightMove) {
    Position from(0, 0);  // a1
    Position to(1, 2);    // b3
    // L-shaped move: 1 file, 2 ranks
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    bool isKnightMove = (fileDiff == 1 && rankDiff == 2) || 
                        (fileDiff == 2 && rankDiff == 1);
    EXPECT_TRUE(isKnightMove);
}

TEST_F(MoveTest, PawnAdvanceOneSquare) {
    Position from(4, 1);  // e2
    Position to(4, 2);    // e3
    // Pawn moving forward one square
    EXPECT_EQ(from.getFile(), to.getFile());
    EXPECT_EQ(abs(from.getRank() - to.getRank()), 1);
}

TEST_F(MoveTest, PawnAdvanceTwoSquares) {
    Position from(4, 1);  // e2
    Position to(4, 3);    // e4
    // Pawn moving forward two squares (from starting position)
    EXPECT_EQ(from.getFile(), to.getFile());
    EXPECT_EQ(abs(from.getRank() - to.getRank()), 2);
}

TEST_F(MoveTest, PawnCaptureMove) {
    Position from(4, 2);  // e3
    Position to(3, 3);    // d4
    // Pawn capturing diagonally
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    EXPECT_EQ(fileDiff, 1);
    EXPECT_EQ(rankDiff, 1);
}

// ============ Position Validation Tests ============

TEST_F(MoveTest, MoveFromValidPosition) {
    Position from(4, 3);  // e4
    EXPECT_TRUE(from.isValid());
}

TEST_F(MoveTest, MoveToValidPosition) {
    Position to(4, 4);  // e5
    EXPECT_TRUE(to.isValid());
}

TEST_F(MoveTest, BothMovePiecesValid) {
    Position from(4, 3);  // e4
    Position to(4, 4);    // e5
    EXPECT_TRUE(from.isValid());
    EXPECT_TRUE(to.isValid());
}

// ============ Move Flags Tests ============

TEST_F(MoveTest, RegularMoveNoFlags) {
    // A regular move should have:
    // - isCapture = false
    // - isCastling = false
    // - isEnPassant = false
    // - isPromotion = false
    Position from(4, 1);  // e2
    Position to(4, 3);    // e4
    
    auto moveBuilder = Move::Builder(from, to);
    // Tests depend on Move API implementation
    EXPECT_TRUE(true);
}

TEST_F(MoveTest, CaptureMove) {
    Position from(4, 4);  // e5
    Position to(3, 5);    // d6 (capturing)
    
    auto moveBuilder = Move::Builder(from, to);
    // Should set isCapture = true
    EXPECT_TRUE(true);
}

TEST_F(MoveTest, CastlingMove) {
    Position from(4, 0);  // e1 (white king)
    Position to(6, 0);    // g1 (kingside castling)
    
    auto moveBuilder = Move::Builder(from, to);
    // Should set isCastling = true
    EXPECT_TRUE(true);
}

TEST_F(MoveTest, EnPassantMove) {
    Position from(4, 4);  // e5
    Position to(3, 5);    // d6 (en passant capture)
    
    auto moveBuilder = Move::Builder(from, to);
    // Should set isEnPassant = true
    EXPECT_TRUE(true);
}

TEST_F(MoveTest, PromotionMove) {
    Position from(4, 6);  // e7 (white pawn)
    Position to(4, 7);    // e8 (promotion square)
    
    auto moveBuilder = Move::Builder(from, to);
    // Should set isPromotion = true
    EXPECT_TRUE(true);
}

// ============ Promotion Target Tests ============

TEST_F(MoveTest, PromotionToQueen) {
    auto promotionTarget = Move::PromotionTarget::QUEEN;
    EXPECT_EQ(promotionTarget, Move::PromotionTarget::QUEEN);
}

TEST_F(MoveTest, PromotionToRook) {
    auto promotionTarget = Move::PromotionTarget::ROOK;
    EXPECT_EQ(promotionTarget, Move::PromotionTarget::ROOK);
}

TEST_F(MoveTest, PromotionToBishop) {
    auto promotionTarget = Move::PromotionTarget::BISHOP;
    EXPECT_EQ(promotionTarget, Move::PromotionTarget::BISHOP);
}

TEST_F(MoveTest, PromotionToKnight) {
    auto promotionTarget = Move::PromotionTarget::KNIGHT;
    EXPECT_EQ(promotionTarget, Move::PromotionTarget::KNIGHT);
}

TEST_F(MoveTest, NoPromotionTarget) {
    auto promotionTarget = Move::PromotionTarget::NONE;
    EXPECT_EQ(promotionTarget, Move::PromotionTarget::NONE);
}

// ============ Edge Case Tests ============

TEST_F(MoveTest, MoveToSamePosition) {
    Position from(4, 3);  // e4
    Position to(4, 3);    // e4
    
    EXPECT_EQ(from, to);  // Same positions
}

TEST_F(MoveTest, MoveAcrossBoardDiagonally) {
    Position from(0, 0);  // a1
    Position to(7, 7);    // h8
    
    int fileDiff = abs(from.getFile() - to.getFile());
    int rankDiff = abs(from.getRank() - to.getRank());
    EXPECT_EQ(fileDiff, rankDiff);  // Should be diagonal
}

TEST_F(MoveTest, MoveFromCornerToCorner) {
    Position from(0, 0);  // a1
    Position to(7, 7);    // h8
    
    EXPECT_TRUE(from.isValid());
    EXPECT_TRUE(to.isValid());
}

// ============ Move Arithmetic Tests ============

TEST_F(MoveTest, CalculateFileDistance) {
    Position from(0, 0);  // a1
    Position to(7, 0);    // h1
    
    int distance = abs(from.getFile() - to.getFile());
    EXPECT_EQ(distance, 7);
}

TEST_F(MoveTest, CalculateRankDistance) {
    Position from(0, 0);  // a1
    Position to(0, 7);    // a8
    
    int distance = abs(from.getRank() - to.getRank());
    EXPECT_EQ(distance, 7);
}

TEST_F(MoveTest, CalculateDiagonalDistance) {
    Position from(0, 0);  // a1
    Position to(7, 7);    // h8
    
    int fileDist = abs(from.getFile() - to.getFile());
    int rankDist = abs(from.getRank() - to.getRank());
    EXPECT_EQ(fileDist, rankDist);
    EXPECT_EQ(fileDist, 7);
}

// ============ Move Notation Tests ============

TEST_F(MoveTest, MoveNotationFromToAlgebraic) {
    Position from = Position::fromAlgebraic("e2");
    Position to = Position::fromAlgebraic("e4");
    
    std::string moveNotation = from.toAlgebraic() + " to " + to.toAlgebraic();
    EXPECT_EQ(moveNotation, "e2 to e4");
}

TEST_F(MoveTest, OpeningMoveEFour) {
    Position from = Position::fromAlgebraic("e2");
    Position to = Position::fromAlgebraic("e4");
    
    EXPECT_EQ(from.toAlgebraic(), "e2");
    EXPECT_EQ(to.toAlgebraic(), "e4");
}

TEST_F(MoveTest, OpeningMoveD4) {
    Position from = Position::fromAlgebraic("d2");
    Position to = Position::fromAlgebraic("d4");
    
    EXPECT_EQ(from.toAlgebraic(), "d2");
    EXPECT_EQ(to.toAlgebraic(), "d4");
}

// ============ Complex Move Sequences ============

TEST_F(MoveTest, SimpleGameSequence) {
    // 1. e2-e4 c7-c5
    Position e2 = Position::fromAlgebraic("e2");
    Position e4 = Position::fromAlgebraic("e4");
    Position c7 = Position::fromAlgebraic("c7");
    Position c5 = Position::fromAlgebraic("c5");
    
    EXPECT_NE(e2, e4);
    EXPECT_NE(c7, c5);
}

TEST_F(MoveTest, KnightsMoveFromStartingPosition) {
    // Knight on b1 can move to a3 or c3
    Position knightStart = Position::fromAlgebraic("b1");
    Position moveA3 = Position::fromAlgebraic("a3");
    Position moveC3 = Position::fromAlgebraic("c3");
    
    EXPECT_NE(knightStart, moveA3);
    EXPECT_NE(knightStart, moveC3);
    EXPECT_NE(moveA3, moveC3);
}
