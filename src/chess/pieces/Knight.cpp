#include "Knight.hpp"
#include "../core/Board.hpp"

namespace chess::pieces {

Knight::Knight(chess::core::Color color, const chess::core::Position& position)
    : Piece(color, position) {
}

std::vector<chess::core::Position> Knight::getLegalDestinations(const chess::core::Board& board) const {
    std::vector<chess::core::Position> destinations;

    // Knight moves in an L-shape: 2 squares in one direction, 1 in the other
    int offsets[][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };

    for (const auto& offset : offsets) {
        int file = position.getFile() + offset[0];
        int rank = position.getRank() + offset[1];

        // Check if position is on the board
        if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
            chess::core::Position pos(file, rank);
            if (board.isEmpty(pos) || board.isEnemyPiece(pos, color)) {
                destinations.push_back(pos);
            }
        }
    }

    return destinations;
}

std::unique_ptr<chess::core::Piece> Knight::copy(const chess::core::Position& pos) const {
    auto copy = std::make_unique<Knight>(color, pos);
    copy->hasMoved = this->hasMoved;
    return copy;
}

char Knight::getSymbol() const {
    return (color == chess::core::Color::WHITE) ? 'N' : 'n';
}

}  // namespace chess::pieces
