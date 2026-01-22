#ifndef CHESS_PGN_PGNPARSER_HPP
#define CHESS_PGN_PGNPARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include "../core/Game.hpp"
#include "../core/Move.hpp"
#include "../core/Player.hpp"

namespace chess::pgn {

/**
 * Structure to hold parsed PGN game data.
 */
struct ParsedGame {
    std::unique_ptr<chess::core::Player> whitePlayer;
    std::unique_ptr<chess::core::Player> blackPlayer;
    std::vector<chess::core::Move> moves;
    std::string resultToken;
};

/**
 * Parses PGN format files and reconstructs games.
 */
class PgnParser {
public:
    /**
     * Parses a PGN file and returns the game data.
     *
     * @param filename the path to the PGN file
     * @return parsed game with players and moves
     * @throws std::runtime_error if file reading or parsing fails
     */
    ParsedGame parse(const std::string& filename) const;
};

}  // namespace chess::pgn

#endif  // CHESS_PGN_PGNPARSER_HPP
