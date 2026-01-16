#include "Pawn.hpp"
#include "../core/Board.hpp"

namespace chess::pieces {

Pawn::Pawn(chess::core::Color color, const chess::core::Position& position)
    : Piece(color, position) {
}

std::vector<chess::core::Position> Pawn::getLegalDestinations(const chess::core::Board& board) const {
    std::vector<chess::core::Position> destinations;
    int direction = (color == chess::core::Color::WHITE) ? 1 : -1;
    int startRank = (color == chess::core::Color::WHITE) ? 1 : 6;

    // Forward move (one square)
    int oneForwardRank = position.getRank() + direction;
    if (oneForwardRank >= 0 && oneForwardRank <= 7) {
        chess::core::Position oneForward(position.getFile(), oneForwardRank);
        if (board.isEmpty(oneForward)) {
            destinations.push_back(oneForward);

            // Forward move (two squares from starting position)
            if (position.getRank() == startRank) {
                int twoForwardRank = position.getRank() + 2 * direction;
                if (twoForwardRank >= 0 && twoForwardRank <= 7) {
                    chess::core::Position twoForward(position.getFile(), twoForwardRank);
                    if (board.isEmpty(twoForward)) {
                        destinations.push_back(twoForward);
                    }
                }
            }
        }
    }

    // Captures (diagonal)
    int captureRank = position.getRank() + direction;
    if (captureRank >= 0 && captureRank <= 7) {
        int captureFiles[] = {position.getFile() - 1, position.getFile() + 1};
        for (int file : captureFiles) {
            if (file >= 0 && file <= 7) {
                chess::core::Position capPos(file, captureRank);
                if (board.isEnemyPiece(capPos, color)) {
                    destinations.push_back(capPos);
                }
            }
        }
    }

    return destinations;
}

std::unique_ptr<chess::core::Piece> Pawn::copy(const chess::core::Position& pos) const {
    auto copy = std::make_unique<Pawn>(color, pos);
    copy->hasMoved = this->hasMoved;
    return copy;
}

char Pawn::getSymbol() const {
    // Return ASCII chess piece symbols
    return (color == chess::core::Color::WHITE) ? 'P' : 'p';
}

}  // namespace chess::pieces
