#include "Move.hpp"
#include "Piece.hpp"

namespace chess::core {

Move::Move(const Position& from, const Position& to, Piece* movedPiece,
           Piece* capturedPiece, bool capture, bool castling, bool enPassant,
           bool promotion, PromotionTarget promTarget)
    : from(from), to(to), movedPiece(movedPiece), capturedPiece(capturedPiece),
      isCapture(capture), isCastling(castling), isEnPassant(enPassant),
      isPromotion(promotion), promotionTarget(promTarget) {
    if (movedPiece == nullptr) {
        throw std::invalid_argument("movedPiece must not be null");
    }
}

Move::Builder::Builder(const Position& from, const Position& to, Piece* movedPiece)
    : from(from), to(to), movedPiece(movedPiece) {
    if (movedPiece == nullptr) {
        throw std::invalid_argument("movedPiece must not be null");
    }
}

Move::Builder& Move::Builder::withCapture(Piece* capturedPiece) {
    this->isCapture = true;
    this->capturedPiece = capturedPiece;
    return *this;
}

Move::Builder& Move::Builder::withCastling() {
    this->isCastling = true;
    return *this;
}

Move::Builder& Move::Builder::withEnPassant() {
    this->isEnPassant = true;
    return *this;
}

Move::Builder& Move::Builder::withPromotion(PromotionTarget target) {
    this->isPromotion = true;
    this->promotionTarget = target;
    return *this;
}

Move Move::Builder::build() const {
    return Move(from, to, movedPiece, capturedPiece, isCapture, isCastling,
                isEnPassant, isPromotion, promotionTarget);
}

}  // namespace chess::core
