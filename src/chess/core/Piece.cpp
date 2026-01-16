#include "Piece.hpp"

namespace chess::core {

Piece::Piece(Color color, const Position& position)
    : color(color), position(position), hasMoved(false) {
}

void Piece::setPosition(const Position& newPosition) {
    this->position = newPosition;
    this->hasMoved = true;
}

}  // namespace chess::core
