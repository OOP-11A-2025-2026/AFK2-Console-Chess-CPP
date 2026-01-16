#include <gtest/gtest.h>
#include "../../../src/chess/core/Color.hpp"

using namespace chess::core;

/**
 * Test Suite for Color enum and utilities
 * Tests color operations and opposite color calculation
 */
class ColorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// ============ Basic Color Tests ============

TEST_F(ColorTest, WhiteColorExists) {
    Color c = Color::WHITE;
    EXPECT_EQ(c, Color::WHITE);
}

TEST_F(ColorTest, BlackColorExists) {
    Color c = Color::BLACK;
    EXPECT_EQ(c, Color::BLACK);
}

TEST_F(ColorTest, WhiteNotEqualsBlack) {
    EXPECT_NE(Color::WHITE, Color::BLACK);
}

TEST_F(ColorTest, BlackNotEqualsWhite) {
    EXPECT_NE(Color::BLACK, Color::WHITE);
}

// ============ Opposite Color Tests ============

TEST_F(ColorTest, OppositeOfWhiteIsBlack) {
    Color opp = opposite(Color::WHITE);
    EXPECT_EQ(opp, Color::BLACK);
}

TEST_F(ColorTest, OppositeOfBlackIsWhite) {
    Color opp = opposite(Color::BLACK);
    EXPECT_EQ(opp, Color::WHITE);
}

TEST_F(ColorTest, DoubleOppositeIsOriginal) {
    Color original = Color::WHITE;
    Color doubled = opposite(opposite(original));
    EXPECT_EQ(doubled, original);

    original = Color::BLACK;
    doubled = opposite(opposite(original));
    EXPECT_EQ(doubled, original);
}

TEST_F(ColorTest, OppositeIsConsistent) {
    // Test multiple invocations return same result
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(opposite(Color::WHITE), Color::BLACK);
        EXPECT_EQ(opposite(Color::BLACK), Color::WHITE);
    }
}

// ============ Color Assignment Tests ============

TEST_F(ColorTest, AssignWhiteColor) {
    Color c;
    c = Color::WHITE;
    EXPECT_EQ(c, Color::WHITE);
}

TEST_F(ColorTest, AssignBlackColor) {
    Color c;
    c = Color::BLACK;
    EXPECT_EQ(c, Color::BLACK);
}

TEST_F(ColorTest, ColorVariableSwitch) {
    Color currentColor = Color::WHITE;
    EXPECT_EQ(currentColor, Color::WHITE);

    currentColor = Color::BLACK;
    EXPECT_EQ(currentColor, Color::BLACK);

    currentColor = opposite(currentColor);
    EXPECT_EQ(currentColor, Color::WHITE);
}

// ============ Color Iteration Tests ============

TEST_F(ColorTest, IterateThroughAllColors) {
    Color colors[] = {Color::WHITE, Color::BLACK};
    
    int count = 0;
    for (const auto& color : colors) {
        if (color == Color::WHITE || color == Color::BLACK) {
            count++;
        }
    }
    
    EXPECT_EQ(count, 2);
}

// ============ Conditional Logic Tests ============

TEST_F(ColorTest, ConditionalOnWhiteColor) {
    Color c = Color::WHITE;
    
    if (c == Color::WHITE) {
        EXPECT_TRUE(true);
    } else {
        EXPECT_TRUE(false);
    }
}

TEST_F(ColorTest, ConditionalOnBlackColor) {
    Color c = Color::BLACK;
    
    if (c == Color::BLACK) {
        EXPECT_TRUE(true);
    } else {
        EXPECT_TRUE(false);
    }
}

TEST_F(ColorTest, TernaryOperatorForColor) {
    Color c = Color::WHITE;
    bool isWhite = (c == Color::WHITE) ? true : false;
    EXPECT_TRUE(isWhite);

    c = Color::BLACK;
    bool isBlack = (c == Color::BLACK) ? true : false;
    EXPECT_TRUE(isBlack);
}

// ============ Turn Management Simulation ============

TEST_F(ColorTest, SimulateTurnSwitch) {
    Color currentTurn = Color::WHITE;
    EXPECT_EQ(currentTurn, Color::WHITE);

    currentTurn = opposite(currentTurn);
    EXPECT_EQ(currentTurn, Color::BLACK);

    currentTurn = opposite(currentTurn);
    EXPECT_EQ(currentTurn, Color::WHITE);

    currentTurn = opposite(currentTurn);
    EXPECT_EQ(currentTurn, Color::BLACK);
}

TEST_F(ColorTest, SimulateGameWithColorCycles) {
    Color color = Color::WHITE;
    
    // Simulate 20 moves (10 full turns)
    for (int i = 0; i < 20; ++i) {
        if (i % 2 == 0) {
            EXPECT_EQ(color, Color::WHITE);
        } else {
            EXPECT_EQ(color, Color::BLACK);
        }
        color = opposite(color);
    }
    
    // After 20 moves, should be back to WHITE's turn
    EXPECT_EQ(color, Color::WHITE);
}

// ============ Utility Function Tests ============

TEST_F(ColorTest, OppositeWithTemporaryColor) {
    EXPECT_EQ(opposite(Color::WHITE), Color::BLACK);
    EXPECT_EQ(opposite(Color::BLACK), Color::WHITE);
}

TEST_F(ColorTest, OppositeChainCalls) {
    Color result = opposite(opposite(opposite(Color::WHITE)));
    EXPECT_EQ(result, Color::BLACK);

    result = opposite(opposite(opposite(opposite(Color::WHITE))));
    EXPECT_EQ(result, Color::WHITE);
}
