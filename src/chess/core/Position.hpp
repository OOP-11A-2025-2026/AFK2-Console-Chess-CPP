#ifndef CHESS_CORE_POSITION_HPP
#define CHESS_CORE_POSITION_HPP

#include <string>
#include <stdexcept>

namespace chess::core {

/**
 * Represents a square on the chess board.
 * File: 0-7 (a-h)
 * Rank: 0-7 (1-8)
 */
class Position {
private:
    int file;
    int rank;

    /**
     * Checks if the given coordinates are valid board positions.
     */
    static bool isValidCoordinates(int file, int rank);

public:
    /**
     * Creates a position at the given file and rank.
     * 
     * @param file the file (0-7, where 0=a, 1=b, ..., 7=h)
     * @param rank the rank (0-7, where 0=1, 1=2, ..., 7=8)
     * @throws std::invalid_argument if position is invalid
     */
    Position(int file, int rank);

    /**
     * Gets the file of this position (0-7, where 0=a and 7=h).
     * 
     * @return the file number
     */
    int getFile() const { return file; }

    /**
     * Gets the rank of this position (0-7, where 0=1 and 7=8).
     * 
     * @return the rank number
     */
    int getRank() const { return rank; }

    /**
     * Checks if the position is within valid board boundaries.
     */
    bool isValid() const;

    /**
     * Converts position to algebraic notation (e.g., "e4").
     */
    std::string toAlgebraic() const;

    /**
     * Creates a Position from algebraic notation (e.g., "e4").
     * 
     * @param square the square in algebraic notation
     * @return the Position object
     * @throws std::invalid_argument if notation is invalid
     */
    static Position fromAlgebraic(const std::string& square);

    /**
     * Equality operator.
     */
    bool operator==(const Position& other) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const Position& other) const;

    /**
     * Hash function for use in std::unordered_map.
     */
    struct Hash {
        std::size_t operator()(const Position& pos) const;
    };
};

}  // namespace chess::core

#endif  // CHESS_CORE_POSITION_HPP
