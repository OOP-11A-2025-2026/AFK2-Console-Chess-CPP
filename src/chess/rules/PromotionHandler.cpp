#include "PromotionHandler.hpp"
#include "../pieces/Pawn.hpp"
#include "../pieces/Queen.hpp"
#include "../pieces/Rook.hpp"
#include "../pieces/Bishop.hpp"
#include "../pieces/Knight.hpp"
#include <memory>

namespace chess::rules {

using namespace chess::core;

bool PromotionHandler::isPromotionMove(const Board& board, const Move& move) {
    Piece* movingPiece = board.getPiece(move.getFrom());
    if (!movingPiece) {
        return false;
    }

    int toRank = move.getTo().getRank();
    bool isWhitePromotion = (toRank == 7);
    bool isBlackPromotion = (toRank == 0);

    return (isWhitePromotion || isBlackPromotion);
}

bool PromotionHandler::isValidPromotion(const Board& board, const Move& move, Color playerColor) {
    Piece* movingPiece = board.getPiece(move.getFrom());
    if (!movingPiece || movingPiece->getColor() != playerColor) {
        return false;
    }

    if (move.getPromotionTarget() == Move::PromotionTarget::NONE) {
        return false;
    }

    return isPromotionMove(board, move);
}

void PromotionHandler::applyPromotion(Board& board, const Move& move) {
    Position from = move.getFrom();
    Position to = move.getTo();

    Piece* pawn = board.getPiece(from);
    if (!pawn) {
        return;
    }

    Color color = pawn->getColor();
    board.removePiece(from);

    std::unique_ptr<Piece> newPiece = std::unique_ptr<Piece>(
        createPromotionPiece(color, to, move.getPromotionTarget())
    );

    board.placePiece(std::move(newPiece), to);
}

Piece* PromotionHandler::createPromotionPiece(Color color, const Position& position,
                                            Move::PromotionTarget target) {
    switch (target) {
        case Move::PromotionTarget::QUEEN:
            return new pieces::Queen(color, position);
        case Move::PromotionTarget::ROOK:
            return new pieces::Rook(color, position);
        case Move::PromotionTarget::BISHOP:
            return new pieces::Bishop(color, position);
        case Move::PromotionTarget::KNIGHT:
            return new pieces::Knight(color, position);
        default:
            return new pieces::Queen(color, position);
    }
}

}
