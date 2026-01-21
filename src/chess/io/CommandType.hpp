#ifndef CHESS_IO_COMMANDTYPE_HPP
#define CHESS_IO_COMMANDTYPE_HPP

namespace chess::io {

enum class CommandType {
    MOVE,
    NEW_GAME,
    LOAD,
    SAVE,
    RESIGN,
    DRAW_OFFER,
    DRAW_ACCEPT,
    UNDO,
    HELP,
    EXIT,
    INVALID
};

}

#endif
