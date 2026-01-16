#ifndef CHESS_PIECES_QUEEN_HPP
#define CHESS_PIECES_QUEEN_HPP

#include "../core/Piece.hpp"

namespace chess::pieces {

class Queen : public chess::core::Piece {
private:
    void addDestinationsInDirection(const chess::core::Board& board,
                                     std::vector<chess::core::Position>& destinations,
                                     int fileDir, int rankDir) const;

public:
    Queen(chess::core::Color color, const chess::core::Position& position);

    std::vector<chess::core::Position> getLegalDestinations(const chess::core::Board& board) const override;
    std::unique_ptr<chess::core::Piece> copy(const chess::core::Position& pos) const override;
    char getSymbol() const override;
    std::string getPieceName() const override { return "Queen"; }
};

}  // namespace chess::pieces

#endif  // CHESS_PIECES_QUEEN_HPP
