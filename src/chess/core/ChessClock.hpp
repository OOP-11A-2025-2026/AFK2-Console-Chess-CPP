#ifndef CHESS_CORE_CHESSCLOCK_HPP
#define CHESS_CORE_CHESSCLOCK_HPP

#include "Color.hpp"
#include <chrono>

namespace chess::core {

/**
 * Represents a chess clock for tracking per-player time.
 */
class ChessClock {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::milliseconds;

    long initialTimeMillis;
    long whiteRemainingMillis;
    long blackRemainingMillis;

    TimePoint lastStartTime;
    Color currentPlayer;
    bool isRunning;

public:
    /**
     * Creates a chess clock with initial time.
     * 
     * @param initialTimeMillis the initial time in milliseconds for each player
     * @throws std::invalid_argument if initialTimeMillis is not positive
     */
    ChessClock(long initialTimeMillis);

    /**
     * Starts the clock for the given player.
     * 
     * @param player the color of the player whose time is starting
     */
    void startTurn(Color player);

    /**
     * Stops the clock for the current player.
     * Deducts the elapsed time from the player's remaining time.
     */
    void stopTurn();

    /**
     * Gets the remaining time for a player in milliseconds.
     * If the clock is currently running for this player, the elapsed time is factored in.
     * 
     * @param player the color of the player
     * @return the remaining time in milliseconds
     */
    long getRemainingTime(Color player) const;

    /**
     * Gets the remaining time in milliseconds as a formatted string (MM:SS).
     * 
     * @param player the color of the player
     * @return formatted time string
     */
    std::string getFormattedTime(Color player) const;

    /**
     * Checks if a player's flag has fallen (time is up).
     * 
     * @param player the color of the player
     * @return true if the player has no time remaining
     */
    bool isFlagFallen(Color player) const;

    /**
     * Gets the initial time in milliseconds.
     */
    long getInitialTime() const { return initialTimeMillis; }

    /**
     * Resets the clock to initial time for both players.
     */
    void reset();
};

}  // namespace chess::core

#endif  // CHESS_CORE_CHESSCLOCK_HPP
