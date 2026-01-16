#include "Queen.hpp"
#include "../core/Board.hpp"

namespace chess::pieces {

Queen::Queen(chess::core::Color color, const chess::core::Position& position)
    : Piece(color, position) {
}

void Queen::addDestinationsInDirection(const chess::core::Board& board,
                                        std::vector<chess::core::Position>& destinations,
                                        int fileDir, int rankDir) const {
    int file = position.getFile() + fileDir;
    int rank = position.getRank() + rankDir;

    while (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
        chess::core::Position pos(file, rank);

        if (board.isEmpty(pos)) {
            destinations.push_back(pos);
        } else if (board.isEnemyPiece(pos, color)) {
            destinations.push_back(pos);
            break;
        } else {
            break;
        }

        file += fileDir;
        rank += rankDir;
    }
}

std::vector<chess::core::Position> Queen::getLegalDestinations(const chess::core::Board& board) const {
    std::vector<chess::core::Position> destinations;

    // Queen moves like both rook and bishop: horizontal, vertical, and diagonal
    int directions[][2] = {
        {1, 0}, {-1, 0},   // Horizontal
        {0, 1}, {0, -1},   // Vertical
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // Diagonal
    };

    for (const auto& dir : directions) {
        addDestinationsInDirection(board, destinations, dir[0], dir[1]);
    }

    return destinations;
}

std::unique_ptr<chess::core::Piece> Queen::copy(const chess::core::Position& pos) const {
    auto copy = std::make_unique<Queen>(color, pos);
    copy->hasMoved = this->hasMoved;
    return copy;
}

char Queen::getSymbol() const {
    return (color == chess::core::Color::WHITE) ? 'Q' : 'q';
}

}  // namespace chess::pieces
