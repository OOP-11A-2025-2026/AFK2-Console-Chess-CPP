#ifndef CHESS_IO_INPUTPARSER_HPP
#define CHESS_IO_INPUTPARSER_HPP

#include "CommandType.hpp"
#include "../core/Move.hpp"
#include "../core/Position.hpp"
#include <string>
#include <optional>

namespace chess::io {

struct ParsedCommand {
    CommandType type;
    std::optional<chess::core::Move> move;
    std::string argument;
};

class InputParser {
public:
    static ParsedCommand parseCommand(const std::string& input);

    static bool isCoordinateNotation(const std::string& input);

    static bool isAlgebraicNotation(const std::string& input);

    static std::optional<chess::core::Move> parseCoordinateMove(const std::string& input);

private:
    static std::string trim(const std::string& str);

    static std::string toLower(const std::string& str);
};

}

#endif
