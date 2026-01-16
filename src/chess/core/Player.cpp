#include "Player.hpp"
#include <sstream>
#include <stdexcept>

namespace chess::core {

Player::Player(const std::string& name, Color color, bool isBot)
    : name(name), color(color), isBot(isBot) {
    if (name.empty()) {
        throw std::invalid_argument("Player name must not be empty");
    }
}

std::string Player::toString() const {
    std::ostringstream oss;
    oss << name << " (" << (color == Color::WHITE ? "WHITE" : "BLACK") << ")";
    if (isBot) {
        oss << " [BOT]";
    }
    return oss.str();
}

}  // namespace chess::core
