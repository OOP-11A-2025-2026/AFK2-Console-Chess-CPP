#ifndef CHESS_RULES_PROMOTIONHANDLER_HPP
#define CHESS_RULES_PROMOTIONHANDLER_HPP

#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include "../core/Color.hpp"
#include "../core/Position.hpp"

namespace chess::rules {

class PromotionHandler {
public:
    static bool isPromotionMove(const chess::core::Board& board,
                               const chess::core::Move& move);

    static bool isValidPromotion(const chess::core::Board& board,
                                const chess::core::Move& move,
                                chess::core::Color playerColor);

    static void applyPromotion(chess::core::Board& board, const chess::core::Move& move);

private:
    static chess::core::Piece* createPromotionPiece(
        chess::core::Color color,
        const chess::core::Position& position,
        chess::core::Move::PromotionTarget target);
};

}

#endif
