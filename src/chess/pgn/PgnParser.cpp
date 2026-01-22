#include "PgnParser.hpp"
#include "PgnIO.hpp"
#include <fstream>

namespace chess::pgn {

ParsedGame PgnParser::parse(const std::string& filename) const {
    // Use PgnIO to load the file into a temporary game
    auto white = std::make_unique<chess::core::Player>("Unknown White", chess::core::Color::WHITE);
    auto black = std::make_unique<chess::core::Player>("Unknown Black", chess::core::Color::BLACK);
    
    chess::core::Game tempGame(std::move(white), std::move(black));
    
    try {
        auto result = PgnIO::loadFromFile(filename, tempGame);
        
        // Extract player names from tags
        std::string whiteName = "Unknown White";
        std::string blackName = "Unknown Black";
        
        if (result.tags.find("White") != result.tags.end()) {
            whiteName = result.tags["White"];
        }
        if (result.tags.find("Black") != result.tags.end()) {
            blackName = result.tags["Black"];
        }
        
        // Create parsed game structure
        ParsedGame parsed;
        parsed.whitePlayer = std::make_unique<chess::core::Player>(whiteName, chess::core::Color::WHITE);
        parsed.blackPlayer = std::make_unique<chess::core::Player>(blackName, chess::core::Color::BLACK);
        parsed.moves = tempGame.getMoveHistory();
        parsed.resultToken = result.resultToken;
        
        return parsed;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse PGN file: " + std::string(e.what()));
    }
}

}  // namespace chess::pgn
