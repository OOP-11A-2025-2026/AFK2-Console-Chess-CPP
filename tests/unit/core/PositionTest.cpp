#include <gtest/gtest.h>
#include "../../../src/chess/core/Position.hpp"

using namespace chess::core;

/**
 * Test Suite for Position class
 * Tests algebraic notation conversion, validation, and equality operations
 */
class PositionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// ============ Construction & Validation Tests ============

TEST_F(PositionTest, ValidPositionConstruction) {
    // Test creation of valid positions
    Position p1(0, 0);  // a1
    EXPECT_EQ(p1.getFile(), 0);
    EXPECT_EQ(p1.getRank(), 0);

    Position p2(7, 7);  // h8
    EXPECT_EQ(p2.getFile(), 7);
    EXPECT_EQ(p2.getRank(), 7);

    Position p3(4, 4);  // e5 (center)
    EXPECT_EQ(p3.getFile(), 4);
    EXPECT_EQ(p3.getRank(), 4);
}

TEST_F(PositionTest, InvalidPositionNegativeFile) {
    EXPECT_THROW(Position(-1, 0), std::invalid_argument);
}

TEST_F(PositionTest, InvalidPositionNegativeRank) {
    EXPECT_THROW(Position(0, -1), std::invalid_argument);
}

TEST_F(PositionTest, InvalidPositionFileOutOfBounds) {
    EXPECT_THROW(Position(8, 0), std::invalid_argument);
    EXPECT_THROW(Position(100, 0), std::invalid_argument);
}

TEST_F(PositionTest, InvalidPositionRankOutOfBounds) {
    EXPECT_THROW(Position(0, 8), std::invalid_argument);
    EXPECT_THROW(Position(0, 100), std::invalid_argument);
}

TEST_F(PositionTest, IsValidReturnsTrue) {
    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            Position pos(file, rank);
            EXPECT_TRUE(pos.isValid()) << "Position (" << file << ", " << rank << ") should be valid";
        }
    }
}

// ============ Algebraic Notation Tests ============

TEST_F(PositionTest, ToAlgebraicA1) {
    Position p(0, 0);
    EXPECT_EQ(p.toAlgebraic(), "a1");
}

TEST_F(PositionTest, ToAlgebraicH8) {
    Position p(7, 7);
    EXPECT_EQ(p.toAlgebraic(), "h8");
}

TEST_F(PositionTest, ToAlgebraicE4) {
    Position p(4, 3);  // e4 (file=4 [e], rank=3 [4])
    EXPECT_EQ(p.toAlgebraic(), "e4");
}

TEST_F(PositionTest, ToAlgebraicD5) {
    Position p(3, 4);  // d5
    EXPECT_EQ(p.toAlgebraic(), "d5");
}

TEST_F(PositionTest, ToAlgebraicAllSquares) {
    const char files[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    const char ranks[] = {'1', '2', '3', '4', '5', '6', '7', '8'};

    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            Position pos(file, rank);
            std::string expected = "";
            expected += files[file];
            expected += ranks[rank];
            EXPECT_EQ(pos.toAlgebraic(), expected);
        }
    }
}

// ============ From Algebraic Tests ============

TEST_F(PositionTest, FromAlgebraicA1) {
    Position p = Position::fromAlgebraic("a1");
    EXPECT_EQ(p.getFile(), 0);
    EXPECT_EQ(p.getRank(), 0);
}

TEST_F(PositionTest, FromAlgebraicH8) {
    Position p = Position::fromAlgebraic("h8");
    EXPECT_EQ(p.getFile(), 7);
    EXPECT_EQ(p.getRank(), 7);
}

TEST_F(PositionTest, FromAlgebraicE4) {
    Position p = Position::fromAlgebraic("e4");
    EXPECT_EQ(p.getFile(), 4);
    EXPECT_EQ(p.getRank(), 3);
}

TEST_F(PositionTest, FromAlgebraicD5) {
    Position p = Position::fromAlgebraic("d5");
    EXPECT_EQ(p.getFile(), 3);
    EXPECT_EQ(p.getRank(), 4);
}

TEST_F(PositionTest, FromAlgebraicAllSquares) {
    const char files[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    const char ranks[] = {'1', '2', '3', '4', '5', '6', '7', '8'};

    for (int file = 0; file < 8; ++file) {
        for (int rank = 0; rank < 8; ++rank) {
            std::string notation = "";
            notation += files[file];
            notation += ranks[rank];

            Position p = Position::fromAlgebraic(notation);
            EXPECT_EQ(p.getFile(), file);
            EXPECT_EQ(p.getRank(), rank);
        }
    }
}

TEST_F(PositionTest, FromAlgebraicInvalidLength) {
    EXPECT_THROW(Position::fromAlgebraic("a"), std::invalid_argument);
    EXPECT_THROW(Position::fromAlgebraic("a1b"), std::invalid_argument);
}

TEST_F(PositionTest, FromAlgebraicInvalidFile) {
    EXPECT_THROW(Position::fromAlgebraic("i1"), std::invalid_argument);
    EXPECT_THROW(Position::fromAlgebraic("z8"), std::invalid_argument);
    EXPECT_THROW(Position::fromAlgebraic("11"), std::invalid_argument);
}

TEST_F(PositionTest, FromAlgebraicInvalidRank) {
    EXPECT_THROW(Position::fromAlgebraic("a0"), std::invalid_argument);
    EXPECT_THROW(Position::fromAlgebraic("a9"), std::invalid_argument);
}

TEST_F(PositionTest, RoundTripAlgebraicConversion) {
    const std::string squares[] = {"a1", "a8", "h1", "h8", "e4", "d5", "b6", "g7"};
    for (const auto& square : squares) {
        Position p = Position::fromAlgebraic(square);
        EXPECT_EQ(p.toAlgebraic(), square);
    }
}

// ============ Equality & Comparison Tests ============

TEST_F(PositionTest, EqualityOperator) {
    Position p1(4, 3);  // e4
    Position p2(4, 3);  // e4
    EXPECT_EQ(p1, p2);
}

TEST_F(PositionTest, EqualityOperatorSelfCompare) {
    Position p(4, 3);
    EXPECT_EQ(p, p);
}

TEST_F(PositionTest, InequalityOperatorDifferentFile) {
    Position p1(3, 3);  // d4
    Position p2(4, 3);  // e4
    EXPECT_NE(p1, p2);
}

TEST_F(PositionTest, InequalityOperatorDifferentRank) {
    Position p1(4, 3);  // e4
    Position p2(4, 4);  // e5
    EXPECT_NE(p1, p2);
}

TEST_F(PositionTest, InequalityOperatorBothDifferent) {
    Position p1(3, 3);  // d4
    Position p2(4, 4);  // e5
    EXPECT_NE(p1, p2);
}

// ============ Hash Function Tests ============

TEST_F(PositionTest, HashConsistency) {
    Position p1(4, 3);
    Position p2(4, 3);
    Position::Hash hasher;

    // Same positions should have same hash
    EXPECT_EQ(hasher(p1), hasher(p2));
}

TEST_F(PositionTest, HashDifferenceForDifferentPositions) {
    Position p1(3, 3);
    Position p2(4, 3);
    Position::Hash hasher;

    // Different positions *might* have different hashes
    // (we can't guarantee this, but it's likely)
    // This test just verifies the hash function works
    auto hash1 = hasher(p1);
    auto hash2 = hasher(p2);
    // The test passes if no exception is thrown
    EXPECT_NE(hash1, hash2);
}

TEST_F(PositionTest, HashFunctionUsableInUnorderedMap) {
    std::unordered_map<Position, std::string, Position::Hash> positionMap;
    
    Position p1(4, 3);  // e4
    Position p2(4, 4);  // e5
    Position p3(4, 3);  // e4 (same as p1)

    positionMap[p1] = "e4";
    positionMap[p2] = "e5";

    EXPECT_EQ(positionMap[p3], "e4");  // p3 should map to same value as p1
    EXPECT_EQ(positionMap.size(), 2);
}

// ============ Edge Cases ============

TEST_F(PositionTest, CornerPositions) {
    Position a1(0, 0);
    Position a8(0, 7);
    Position h1(7, 0);
    Position h8(7, 7);

    EXPECT_EQ(a1.toAlgebraic(), "a1");
    EXPECT_EQ(a8.toAlgebraic(), "a8");
    EXPECT_EQ(h1.toAlgebraic(), "h1");
    EXPECT_EQ(h8.toAlgebraic(), "h8");
}

TEST_F(PositionTest, CenterPositions) {
    Position d4(3, 3);
    Position e4(4, 3);
    Position d5(3, 4);
    Position e5(4, 4);

    EXPECT_EQ(d4.toAlgebraic(), "d4");
    EXPECT_EQ(e4.toAlgebraic(), "e4");
    EXPECT_EQ(d5.toAlgebraic(), "d5");
    EXPECT_EQ(e5.toAlgebraic(), "e5");
}
