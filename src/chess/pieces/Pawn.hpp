#ifndef CHESS_PIECES_PAWN_HPP
#define CHESS_PIECES_PAWN_HPP

#include "../core/Piece.hpp"

namespace chess::pieces {

class Pawn : public chess::core::Piece {
public:
    Pawn(chess::core::Color color, const chess::core::Position& position);

    std::vector<chess::core::Position> getLegalDestinations(const chess::core::Board& board) const override;
    std::unique_ptr<chess::core::Piece> copy(const chess::core::Position& pos) const override;
    char getSymbol() const override;
    std::string getPieceName() const override { return "Pawn"; }
};

}  // namespace chess::pieces

#endif  // CHESS_PIECES_PAWN_HPP
