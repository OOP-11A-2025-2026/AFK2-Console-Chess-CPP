#ifndef CHESS_CORE_PLAYER_HPP
#define CHESS_CORE_PLAYER_HPP

#include "Color.hpp"
#include <string>

namespace chess::core {

/**
 * Represents a player in a chess game.
 */
class Player {
private:
    std::string name;
    Color color;
    bool isBot;

public:
    /**
     * Creates a player.
     * 
     * @param name the player's name (must not be empty)
     * @param color the player's color
     * @param isBot true if this is a bot player
     * @throws std::invalid_argument if name is empty or color is invalid
     */
    Player(const std::string& name, Color color, bool isBot = false);

    /**
     * Gets the player's name.
     * 
     * @return the player's name (non-empty string)
     */
    const std::string& getName() const { return name; }

    /**
     * Gets the player's color in this game.
     * 
     * @return the player's color (WHITE or BLACK)
     */
    Color getColor() const { return color; }

    /**
     * Checks if this player is a bot AI player.
     * 
     * @return true if this is a bot/AI player, false if human
     */
    bool isBot_() const { return isBot; }

    /**
     * Returns a string representation of the player.
     * Format: "Name (COLOR)" or "Name (COLOR) [BOT]" if it's a bot.
     */
    std::string toString() const;
};

}  // namespace chess::core

#endif  // CHESS_CORE_PLAYER_HPP
