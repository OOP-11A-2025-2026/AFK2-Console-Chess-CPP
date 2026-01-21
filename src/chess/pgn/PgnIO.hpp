#ifndef CHESS_PGN_PGNIO_HPP
#define CHESS_PGN_PGNIO_HPP

#include "../core/Game.hpp"
#include <string>
#include <unordered_map>

namespace chess::pgn {

/**
 * Result information returned after loading a PGN file.
 */
struct PgnLoadResult {
    std::unordered_map<std::string, std::string> tags;
    std::string resultToken;
};

/**
 * Utility class for reading/writing PGN files into the core Game model.
 * Parsing is intentionally permissive and focuses on common SAN tokens,
 * move numbers, and a minimal tag set.
 */
class PgnIO {
public:
    /**
     * Loads a PGN file, applies the moves to the provided game instance,
     * and returns the parsed tags/result token.
     *
     * The game is reset before applying the PGN moves.
     *
     * @param path path to the PGN file
     * @param game game instance to populate
     * @return parsed tags and the final result token (e.g. "1-0", "1/2-1/2", "*")
     * @throws std::runtime_error on parse or validation errors
     */
    static PgnLoadResult loadFromFile(const std::string& path, chess::core::Game& game);

    /**
     * Saves the current game state as a PGN file.
     *
     * @param path target PGN path
     * @param game source game instance
     * @param extraTags optional tags to write/override (Result is derived unless provided)
     * @throws std::runtime_error if writing fails
     */
    static void saveToFile(const std::string& path, const chess::core::Game& game,
                           const std::unordered_map<std::string, std::string>& extraTags = {});
};

}  // namespace chess::pgn

#endif  // CHESS_PGN_PGNIO_HPP
