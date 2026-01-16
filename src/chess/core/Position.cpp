#include "Position.hpp"

namespace chess::core {

Position::Position(int file, int rank) : file(file), rank(rank) {
    if (!isValidCoordinates(file, rank)) {
        throw std::invalid_argument("Invalid position: file=" + std::to_string(file) + 
                                   ", rank=" + std::to_string(rank));
    }
}

bool Position::isValidCoordinates(int file, int rank) {
    return file >= 0 && file <= 7 && rank >= 0 && rank <= 7;
}

bool Position::isValid() const {
    return isValidCoordinates(file, rank);
}

std::string Position::toAlgebraic() const {
    char fileLetter = static_cast<char>('a' + file);
    int rankNumber = rank + 1;
    return std::string(1, fileLetter) + std::to_string(rankNumber);
}

Position Position::fromAlgebraic(const std::string& square) {
    if (square.length() != 2) {
        throw std::invalid_argument("Invalid algebraic notation: " + square);
    }

    char fileLetter = square[0];
    char rankLetter = square[1];

    if (fileLetter < 'a' || fileLetter > 'h') {
        throw std::invalid_argument("Invalid file: " + std::string(1, fileLetter));
    }
    if (rankLetter < '1' || rankLetter > '8') {
        throw std::invalid_argument("Invalid rank: " + std::string(1, rankLetter));
    }

    int file = fileLetter - 'a';
    int rank = rankLetter - '1';

    return Position(file, rank);
}

bool Position::operator==(const Position& other) const {
    return this->file == other.file && this->rank == other.rank;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

std::size_t Position::Hash::operator()(const Position& pos) const {
    return std::hash<int>()(pos.getFile()) ^ (std::hash<int>()(pos.getRank()) << 1);
}

}  // namespace chess::core
