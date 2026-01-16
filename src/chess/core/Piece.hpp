#ifndef CHESS_CORE_PIECE_HPP
#define CHESS_CORE_PIECE_HPP

#include "Color.hpp"
#include "Position.hpp"
#include <vector>
#include <memory>

namespace chess::core {

// Forward declaration
class Board;

/**
 * Abstract base class for all chess pieces.
 * Each piece type implements its own movement pattern.
 */
class Piece {
protected:
    Color color;
    Position position;
    bool hasMoved;

protected:
    /**
     * Protected constructor for subclasses.
     * 
     * @param color the color of the piece
     * @param position the initial position
     */
    Piece(Color color, const Position& position);

public:
    virtual ~Piece() = default;

    /**
     * Gets the color of this piece.
     * 
     * @return the color (WHITE or BLACK)
     */
    Color getColor() const { return color; }

    /**
     * Gets the current position of this piece on the board.
     * 
     * @return the piece's position
     */
    Position getPosition() const { return position; }

    /**
     * Checks if this piece has moved from its starting position.
     * This is used for special rules like castling and pawn double moves.
     * 
     * @return true if the piece has moved, false otherwise
     */
    bool hasMoved_() const { return hasMoved; }

    /**
     * Sets the position of the piece. Called by the board when moving pieces.
     */
    void setPosition(const Position& newPosition);

    /**
     * Returns all pseudo-legal destinations for this piece from its current position.
     * Pseudo-legal means: respects piece movement rules, but doesn't check if it leaves
     * the king in check (that validation happens in MoveValidator).
     * 
     * @param board the current board state
     * @return vector of legal destination positions
     */
    virtual std::vector<Position> getLegalDestinations(const Board& board) const = 0;

    /**
     * Creates a copy of this piece at a given position.
     * Preserves the movement history (hasMoved flag) of the original piece.
     * 
     * @param pos the position for the copied piece
     * @return a new Piece instance with the same color and movement history
     */
    virtual std::unique_ptr<Piece> copy(const Position& pos) const = 0;

    /**
     * Gets the Unicode symbol representing this piece.
     * 
     * @return the Unicode character representing this piece
     */
    virtual char getSymbol() const = 0;

    /**
     * Gets the piece type as a string (e.g., "Pawn", "Rook").
     */
    virtual std::string getPieceName() const = 0;
};

}  // namespace chess::core

#endif  // CHESS_CORE_PIECE_HPP
