#ifndef CHESS_CORE_MOVE_HPP
#define CHESS_CORE_MOVE_HPP

#include "Position.hpp"
#include <memory>

namespace chess::core {

// Forward declaration
class Piece;

/**
 * Represents a move in chess, storing the source and destination,
 * the pieces involved, and special move flags.
 */
class Move {
public:
    enum class PromotionTarget {
        NONE,
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT
    };

private:
    Position from;
    Position to;
    Piece* movedPiece;
    Piece* capturedPiece;

    bool isCapture;
    bool isCastling;
    bool isEnPassant;
    bool isPromotion;
    PromotionTarget promotionTarget;

    // Private constructor - use Builder
    Move(const Position& from, const Position& to, Piece* movedPiece, 
         Piece* capturedPiece, bool capture, bool castling, bool enPassant,
         bool promotion, PromotionTarget promTarget);

public:
    // Builder class for constructing Move objects
    class Builder {
    private:
        Position from;
        Position to;
        Piece* movedPiece;
        Piece* capturedPiece = nullptr;
        bool isCapture = false;
        bool isCastling = false;
        bool isEnPassant = false;
        bool isPromotion = false;
        PromotionTarget promotionTarget = PromotionTarget::NONE;

    public:
        Builder(const Position& from, const Position& to, Piece* movedPiece);

        Builder& withCapture(Piece* capturedPiece);
        Builder& withCastling();
        Builder& withEnPassant();
        Builder& withPromotion(PromotionTarget target);

        Move build() const;
    };

    // Getters
    Position getFrom() const { return from; }
    Position getTo() const { return to; }
    Piece* getMovedPiece() const { return movedPiece; }
    Piece* getCapturedPiece() const { return capturedPiece; }

    bool isCapture_() const { return isCapture; }
    bool isCastling_() const { return isCastling; }
    bool isEnPassant_() const { return isEnPassant; }
    bool isPromotion_() const { return isPromotion; }
    PromotionTarget getPromotionTarget() const { return promotionTarget; }
};

}  // namespace chess::core

#endif  // CHESS_CORE_MOVE_HPP
