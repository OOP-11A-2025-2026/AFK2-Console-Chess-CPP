#ifndef CHESS_RULES_CASTLINGHANDLER_HPP
#define CHESS_RULES_CASTLINGHANDLER_HPP

#include "../core/Board.hpp"
#include "../core/Move.hpp"
#include "../core/Color.hpp"

namespace chess::rules {

class CastlingHandler {
public:
    static bool isValidCastle(const chess::core::Board& board,
                             const chess::core::Move& move,
                             chess::core::Color playerColor);

    static bool canCastleKingSide(const chess::core::Board& board,
                                 chess::core::Color color);

    static bool canCastleQueenSide(const chess::core::Board& board,
                                  chess::core::Color color);

    static void applyCastling(chess::core::Board& board,
                             const chess::core::Move& move);
};

}

#endif
