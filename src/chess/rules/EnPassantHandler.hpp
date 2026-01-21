#ifndef CHESS_RULES_ENPASSANTHANDLER_HPP
#define CHESS_RULES_ENPASSANTHANDLER_HPP

#include "../core/Board.hpp"
#include "../core/Position.hpp"
#include "../core/Move.hpp"
#include "../core/Color.hpp"

namespace chess::rules {

class EnPassantHandler {
public:
    static bool isValidEnPassant(const chess::core::Board& board,
                                const chess::core::Move& move,
                                chess::core::Color playerColor,
                                const chess::core::Move* lastMove);

    static chess::core::Position getEnPassantTarget(const chess::core::Move& pawnMove);

    static void applyEnPassant(chess::core::Board& board, const chess::core::Move& move);
};

}

#endif
