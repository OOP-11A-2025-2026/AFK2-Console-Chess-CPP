#include "Bishop.hpp"
#include "../core/Board.hpp"

namespace chess::pieces {

Bishop::Bishop(chess::core::Color color, const chess::core::Position& position)
    : Piece(color, position) {
}

void Bishop::addDestinationsInDirection(const chess::core::Board& board,
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

std::vector<chess::core::Position> Bishop::getLegalDestinations(const chess::core::Board& board) const {
    std::vector<chess::core::Position> destinations;

    // Diagonal directions
    int directions[][2] = {
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };

    for (const auto& dir : directions) {
        addDestinationsInDirection(board, destinations, dir[0], dir[1]);
    }

    return destinations;
}

std::unique_ptr<chess::core::Piece> Bishop::copy(const chess::core::Position& pos) const {
    auto copy = std::make_unique<Bishop>(color, pos);
    copy->hasMoved = this->hasMoved;
    return copy;
}

char Bishop::getSymbol() const {
    return (color == chess::core::Color::WHITE) ? 'B' : 'b';
}

}  // namespace chess::pieces
