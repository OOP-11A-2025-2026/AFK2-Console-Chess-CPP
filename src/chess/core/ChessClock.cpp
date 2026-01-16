#include "ChessClock.hpp"
#include <stdexcept>
#include <iomanip>
#include <sstream>

namespace chess::core {

ChessClock::ChessClock(long initialTimeMillis)
    : initialTimeMillis(initialTimeMillis),
      whiteRemainingMillis(initialTimeMillis),
      blackRemainingMillis(initialTimeMillis),
      currentPlayer(Color::WHITE),
      isRunning(false) {
    if (initialTimeMillis <= 0) {
        throw std::invalid_argument("Initial time must be positive");
    }
}

void ChessClock::startTurn(Color player) {
    if (isRunning && currentPlayer == player) {
        return;  // Already running for this player
    }

    if (isRunning) {
        stopTurn();  // Stop the previous player's time
    }

    currentPlayer = player;
    isRunning = true;
    lastStartTime = Clock::now();
}

void ChessClock::stopTurn() {
    if (!isRunning) {
        return;
    }

    auto now = Clock::now();
    auto elapsed = std::chrono::duration_cast<Duration>(now - lastStartTime).count();

    if (currentPlayer == Color::WHITE) {
        whiteRemainingMillis -= elapsed;
        if (whiteRemainingMillis < 0) {
            whiteRemainingMillis = 0;
        }
    } else {
        blackRemainingMillis -= elapsed;
        if (blackRemainingMillis < 0) {
            blackRemainingMillis = 0;
        }
    }

    isRunning = false;
}

long ChessClock::getRemainingTime(Color player) const {
    long remaining = (player == Color::WHITE) ? whiteRemainingMillis : blackRemainingMillis;

    // If the clock is running for this player, subtract elapsed time
    if (isRunning && currentPlayer == player) {
        auto now = Clock::now();
        auto elapsed = std::chrono::duration_cast<Duration>(now - lastStartTime).count();
        remaining -= elapsed;
        if (remaining < 0) {
            remaining = 0;
        }
    }

    return remaining;
}

std::string ChessClock::getFormattedTime(Color player) const {
    long milliseconds = getRemainingTime(player);
    long minutes = milliseconds / 60000;
    long seconds = (milliseconds % 60000) / 1000;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setw(2) << seconds;
    return oss.str();
}

bool ChessClock::isFlagFallen(Color player) const {
    return getRemainingTime(player) <= 0;
}


void ChessClock::reset() {
    stopTurn();
    whiteRemainingMillis = initialTimeMillis;
    blackRemainingMillis = initialTimeMillis;
}

}  // namespace chess::core
