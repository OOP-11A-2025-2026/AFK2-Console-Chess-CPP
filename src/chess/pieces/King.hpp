#ifndef CHESS_PIECES_KING_HPP
#define CHESS_PIECES_KING_HPP

#include "../core/Piece.hpp"

namespace chess::pieces {

class King : public chess::core::Piece {
public:
    King(chess::core::Color color, const chess::core::Position& position);

    std::vector<chess::core::Position> getLegalDestinations(const chess::core::Board& board) const override;
    std::unique_ptr<chess::core::Piece> copy(const chess::core::Position& pos) const override;
    char getSymbol() const override;
    std::string getPieceName() const override { return "King"; }
};

}  // namespace chess::pieces

#endif  // CHESS_PIECES_KING_HPP
