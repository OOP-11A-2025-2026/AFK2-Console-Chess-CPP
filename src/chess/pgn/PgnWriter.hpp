#ifndef CHESS_PGN_PGNWRITER_HPP
#define CHESS_PGN_PGNWRITER_HPP

#include <string>
#include <unordered_map>
#include "../core/Game.hpp"

namespace chess::pgn {

/**
 * Writes chess games to PGN format files.
 */
class PgnWriter {
public:
    /**
     * Writes a game to a PGN file.
     *
     * @param filename the path to write to
     * @param game the game to save
     * @param extraTags optional additional PGN tags to include
     * @throws std::runtime_error if file writing fails
     */
    void write(const std::string& filename, const chess::core::Game& game,
               const std::unordered_map<std::string, std::string>& extraTags = {}) const;
};

}  // namespace chess::pgn

#endif  // CHESS_PGN_PGNWRITER_HPP
