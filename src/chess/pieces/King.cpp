#include "King.hpp"
#include "../core/Board.hpp"

namespace chess::pieces {

King::King(chess::core::Color color, const chess::core::Position& position)
    : Piece(color, position) {
}

std::vector<chess::core::Position> King::getLegalDestinations(const chess::core::Board& board) const {
    std::vector<chess::core::Position> destinations;

    // King moves one square in any direction
    int directions[][2] = {
        {1, 0}, {-1, 0},     // Horizontal
        {0, 1}, {0, -1},     // Vertical
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // Diagonal
    };

    for (const auto& dir : directions) {
        int file = position.getFile() + dir[0];
        int rank = position.getRank() + dir[1];

        // Check if position is on the board
        if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
            chess::core::Position pos(file, rank);
            if (board.isEmpty(pos) || board.isEnemyPiece(pos, color)) {
                destinations.push_back(pos);
            }
        }
    }

    // TODO: Add castling logic when CastlingHandler is implemented
    // Castling moves would be added here, but they require checking:
    // - King hasn't moved
    // - Rook hasn't moved
    // - Path is clear
    // - King doesn't move through check

    return destinations;
}

std::unique_ptr<chess::core::Piece> King::copy(const chess::core::Position& pos) const {
    auto copy = std::make_unique<King>(color, pos);
    copy->hasMoved = this->hasMoved;
    return copy;
}

char King::getSymbol() const {
    return (color == chess::core::Color::WHITE) ? 'K' : 'k';
}

}  // namespace chess::pieces
