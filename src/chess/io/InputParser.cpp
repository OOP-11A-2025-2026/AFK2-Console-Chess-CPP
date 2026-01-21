#include "InputParser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace chess::io {

using namespace chess::core;

std::string InputParser::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::string InputParser::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c) { return std::tolower(c); });
    return result;
}

ParsedCommand InputParser::parseCommand(const std::string& input) {
    std::string trimmed = trim(input);
    std::string lower = toLower(trimmed);

    if (lower == "new" || lower == "new game") {
        return {CommandType::NEW_GAME, std::nullopt, ""};
    }
    if (lower == "help") {
        return {CommandType::HELP, std::nullopt, ""};
    }
    if (lower == "exit" || lower == "quit") {
        return {CommandType::EXIT, std::nullopt, ""};
    }
    if (lower == "undo") {
        return {CommandType::UNDO, std::nullopt, ""};
    }
    if (lower == "resign") {
        return {CommandType::RESIGN, std::nullopt, ""};
    }
    if (lower == "draw" || lower == "draw offer") {
        return {CommandType::DRAW_OFFER, std::nullopt, ""};
    }
    if (lower == "accept" || lower == "accept draw") {
        return {CommandType::DRAW_ACCEPT, std::nullopt, ""};
    }

    if (lower.find("save") == 0) {
        std::string filename = trim(lower.substr(4));
        return {CommandType::SAVE, std::nullopt, filename};
    }
    if (lower.find("load") == 0) {
        std::string filename = trim(lower.substr(4));
        return {CommandType::LOAD, std::nullopt, filename};
    }

    if (isCoordinateNotation(trimmed)) {
        auto move = parseCoordinateMove(trimmed);
        if (move.has_value()) {
            return {CommandType::MOVE, move, ""};
        }
    }

    if (isAlgebraicNotation(trimmed)) {
        return {CommandType::MOVE, std::nullopt, trimmed};
    }

    return {CommandType::INVALID, std::nullopt, ""};
}

bool InputParser::isCoordinateNotation(const std::string& input) {
    std::string trimmed = trim(input);
    if (trimmed.length() < 5) return false;

    char f1 = trimmed[0];
    char r1 = trimmed[1];
    char f2 = trimmed[3];
    char r2 = trimmed[4];

    return (f1 >= 'a' && f1 <= 'h') && (r1 >= '1' && r1 <= '8') &&
           (f2 >= 'a' && f2 <= 'h') && (r2 >= '1' && r2 <= '8');
}

std::optional<Move> InputParser::parseCoordinateMove(const std::string& input) {
    std::string trimmed = trim(input);

    if (!isCoordinateNotation(trimmed)) {
        return std::nullopt;
    }

    try {
        int fromFile = trimmed[0] - 'a';
        int fromRank = trimmed[1] - '1';
        int toFile = trimmed[3] - 'a';
        int toRank = trimmed[4] - '1';

        Position from(fromFile, fromRank);
        Position to(toFile, toRank);

        Move::Builder builder(from, to, nullptr);
        return builder.build();
    } catch (...) {
        return std::nullopt;
    }
}

bool InputParser::isAlgebraicNotation(const std::string& input) {
    std::string trimmed = trim(input);
    if (trimmed.length() < 2) return false;

    char first = trimmed[0];
    bool startsWithPiece = (first == 'K' || first == 'Q' || first == 'R' ||
                           first == 'B' || first == 'N');

    return startsWithPiece || (first >= 'a' && first <= 'h');
}

}
