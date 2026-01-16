#ifndef CHESS_CORE_COLOR_HPP
#define CHESS_CORE_COLOR_HPP

namespace chess::core {

/**
 * Enum representing the two colors in chess.
 */
enum class Color {
    WHITE,
    BLACK
};

/**
 * Returns the opposite color.
 * @param color The color to invert
 * @return WHITE if input is BLACK, BLACK if input is WHITE
 */
inline Color opposite(Color color) {
    return color == Color::WHITE ? Color::BLACK : Color::WHITE;
}

}  // namespace chess::core

#endif  // CHESS_CORE_COLOR_HPP
