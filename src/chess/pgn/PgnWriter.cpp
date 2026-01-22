#include "PgnWriter.hpp"
#include "../util/AlgebraicNotationUtil.hpp"
#include <fstream>
#include <iomanip>
#include <ctime>

namespace chess::pgn {

void PgnWriter::write(const std::string& filename, const chess::core::Game& game,
                      const std::unordered_map<std::string, std::string>& extraTags) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open PGN file for writing: " + filename);
    }

    // Write tags
    std::unordered_map<std::string, std::string> tags{
        {"Event", "AFK2 Console Chess"},
        {"White", game.getWhitePlayer().getName()},
        {"Black", game.getBlackPlayer().getName()},
        {"Result", "*"}
    };

    // Add extra tags
    for (const auto& kv : extraTags) {
        tags[kv.first] = kv.second;
    }

    for (const auto& kv : tags) {
        file << "[" << kv.first << " \"" << kv.second << "\"]\n";
    }
    file << "\n";

    // Write moves
    const auto& history = game.getMoveHistory();
    chess::core::Board replayBoard;
    
    size_t moveNumber = 1;
    for (size_t i = 0; i < history.size(); ++i) {
        if (i % 2 == 0) {
            file << moveNumber << ". ";
            ++moveNumber;
        }

        const auto& move = history[i];
        std::string notation = chess::util::AlgebraicNotationUtil::toAlgebraic(replayBoard, move);
        file << notation << " ";
        
        // Update replay board
        replayBoard.movePiece(move.getFrom(), move.getTo());
    }

    file << "*\n";
    file.close();
}

}  // namespace chess::pgn
