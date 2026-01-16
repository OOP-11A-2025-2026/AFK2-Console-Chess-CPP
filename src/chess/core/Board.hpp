#ifndef CHESS_CORE_BOARD_HPP
#define CHESS_CORE_BOARD_HPP

#include "Color.hpp"
#include "Position.hpp"
#include "Piece.hpp"
#include <unordered_map>
#include <memory>

namespace chess::core {

/**
 * Represents the 8x8 chess board.
 * The board is the single source of truth for the game state.
 */
class Board {
private:
    static constexpr int BOARD_SIZE = 8;

    // Map from Position to Piece
    std::unordered_map<Position, std::unique_ptr<Piece>, Position::Hash> pieces;

    Position whiteKingPosition;
    Position blackKingPosition;

    /**
     * Initializes the board with pieces in their starting positions.
     */
    void initializeStartingPosition();

public:
    /**
     * Creates a new board with all pieces in starting position.
     */
    Board();

    /**
     * Copy constructor for deep copying the board.
     */
    Board(const Board& other);

    /**
     * Move constructor.
     */
    Board(Board&& other) noexcept;

    /**
     * Assignment operator.
     */
    Board& operator=(const Board& other);

    /**
     * Move assignment operator.
     */
    Board& operator=(Board&& other) noexcept;

    /**
     * Virtual destructor.
     */
    ~Board() = default;

    /**
     * Gets the piece at a position, or nullptr if empty.
     * 
     * @param pos the position to check
     * @return the piece at the position, or nullptr if the square is empty
     */
    Piece* getPiece(const Position& pos) const;

    /**
     * Gets a non-owning reference to a piece (for const access).
     */
    const Piece* getPieceConst(const Position& pos) const;

    /**
     * Checks if a square is empty.
     * 
     * @param pos the position to check
     * @return true if the square is empty
     */
    bool isEmpty(const Position& pos) const;

    /**
     * Checks if a square contains an enemy piece.
     * 
     * @param pos the position to check
     * @param color the color of the piece we're checking against
     * @return true if there is an enemy piece at the position
     */
    bool isEnemyPiece(const Position& pos, Color color) const;

    /**
     * Checks if a square contains a friendly piece.
     * 
     * @param pos the position to check
     * @param color the color of the piece we're checking against
     * @return true if there is a friendly piece at the position
     */
    bool isFriendlyPiece(const Position& pos, Color color) const;

    /**
     * Places a piece on the board at the specified position.
     * Updates king position tracking if a king is being placed.
     * Takes ownership of the piece.
     * 
     * @param piece the piece to place (moved)
     * @param position the position to place the piece at
     */
    void placePiece(std::unique_ptr<Piece> piece, const Position& position);

    /**
     * Removes a piece from the board at the specified position.
     * Returns the removed piece.
     * 
     * @param position the position to remove the piece from
     * @return the removed piece (unique_ptr), or nullptr if no piece
     */
    std::unique_ptr<Piece> removePiece(const Position& position);

    /**
     * Moves a piece from one position to another.
     * Updates king position tracking and returns the captured piece, if any.
     * 
     * @param from the source position
     * @param to the destination position
     * @return the captured piece (unique_ptr), or nullptr if this is not a capture
     */
    std::unique_ptr<Piece> movePiece(const Position& from, const Position& to);

    /**
     * Gets the position of the white king.
     */
    Position getWhiteKingPosition() const { return whiteKingPosition; }

    /**
     * Gets the position of the black king.
     */
    Position getBlackKingPosition() const { return blackKingPosition; }

    /**
     * Gets the king position for a given color.
     */
    Position getKingPosition(Color color) const;

    /**
     * Gets all pieces of a given color.
     */
    std::vector<Piece*> getPiecesOfColor(Color color) const;
};

}  // namespace chess::core

#endif  // CHESS_CORE_BOARD_HPP
