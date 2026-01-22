#include "AlgebraicNotationUtil.hpp"
#include "../pieces/Pawn.hpp"
#include "../pieces/Knight.hpp"
#include "../pieces/Bishop.hpp"
#include "../pieces/Rook.hpp"
#include "../pieces/Queen.hpp"
#include "../pieces/King.hpp"
#include <sstream>
#include <algorithm>

namespace chess::util {

std::string AlgebraicNotationUtil::toLongAlgebraic(const chess::core::Move& move) {
    return move.getFrom().toAlgebraic() + move.getTo().toAlgebraic();
}

char AlgebraicNotationUtil::getPieceSymbol(const chess::core::Piece* piece) {
    if (!piece) return ' ';

    std::string name = piece->getPieceName();
    if (name == "Pawn") return 'P';
    if (name == "Knight") return 'N';
    if (name == "Bishop") return 'B';
    if (name == "Rook") return 'R';
    if (name == "Queen") return 'Q';
    if (name == "King") return 'K';
    return '?';
}

std::string AlgebraicNotationUtil::toAlgebraic(const chess::core::Board& /* board */,
                                                const chess::core::Move& move) {
    std::ostringstream oss;

    chess::core::Piece* piece = move.getMovedPiece();
    if (!piece) {
        return toLongAlgebraic(move);
    }

    std::string pieceName = piece->getPieceName();

    // Pawns are special: they use the destination square or capture notation
    if (pieceName == "Pawn") {
        if (move.isCapture_()) {
            // Pawn capture: e.g., "exd5"
            char fromFile = static_cast<char>('a' + move.getFrom().getFile());
            oss << fromFile << "x" << move.getTo().toAlgebraic();
        } else {
            // Pawn move: just the destination square
            oss << move.getTo().toAlgebraic();
        }
        if (move.isPromotion_()) {
            // Add promotion piece: e.g., "e8=Q"
            char promoPiece = 'Q';  // Default to queen
            if (move.getPromotionTarget() == chess::core::Move::PromotionTarget::ROOK) {
                promoPiece = 'R';
            } else if (move.getPromotionTarget() == chess::core::Move::PromotionTarget::BISHOP) {
                promoPiece = 'B';
            } else if (move.getPromotionTarget() == chess::core::Move::PromotionTarget::KNIGHT) {
                promoPiece = 'N';
            }
            oss << "=" << promoPiece;
        }
    } else {
        // Other pieces: e.g., "Nf3", "Bxc5"
        char symbol = getPieceSymbol(piece);
        oss << symbol;

        if (move.isCapture_()) {
            oss << "x";
        }
        oss << move.getTo().toAlgebraic();
    }

    return oss.str();
}

std::unique_ptr<chess::core::Move> AlgebraicNotationUtil::parseMove(const std::string& notation,
                                                                      const chess::core::Board& board,
                                                                      chess::core::Color playerColor) {
    // Try to parse as long algebraic (e2e4)
    if (notation.length() == 4) {
        try {
            chess::core::Position from = chess::core::Position::fromAlgebraic(notation.substr(0, 2));
            chess::core::Position to = chess::core::Position::fromAlgebraic(notation.substr(2, 2));

            chess::core::Piece* piece = board.getPiece(from);
            if (!piece || piece->getColor() != playerColor) {
                return nullptr;
            }

            chess::core::Piece* captured = board.getPiece(to);

            auto builder = chess::core::Move::Builder(from, to, piece);
            if (captured) {
                builder.withCapture(captured);
            }

            return std::make_unique<chess::core::Move>(builder.build());
        } catch (...) {
            // Fall through to other formats
        }
    }

    // Try to parse as coordinate notation with space (e2 e4)
    size_t spacePos = notation.find(' ');
    if (spacePos != std::string::npos) {
        try {
            chess::core::Position from = chess::core::Position::fromAlgebraic(notation.substr(0, spacePos));
            chess::core::Position to = chess::core::Position::fromAlgebraic(notation.substr(spacePos + 1));

            chess::core::Piece* piece = board.getPiece(from);
            if (!piece || piece->getColor() != playerColor) {
                return nullptr;
            }

            chess::core::Piece* captured = board.getPiece(to);

            auto builder = chess::core::Move::Builder(from, to, piece);
            if (captured) {
                builder.withCapture(captured);
            }

            return std::make_unique<chess::core::Move>(builder.build());
        } catch (...) {
            return nullptr;
        }
    }

    // Try to parse as short algebraic notation (e.g., "e4", "Nf3", "exd5", "Bxc5")
    try {
        // Determine if it's a piece move or pawn move
        char first = notation[0];
        char pieceSymbol = ' ';
        size_t movePartStart = 0;

        if (first == 'K' || first == 'Q' || first == 'R' || first == 'B' || first == 'N') {
            pieceSymbol = first;
            movePartStart = 1;
        }

        // Extract the destination square (last 2 characters before any promotion)
        size_t promotionPos = notation.find('=');
        size_t endPos = (promotionPos != std::string::npos) ? promotionPos : notation.length();
        
        if (endPos < 2) {
            return nullptr;
        }

        std::string destStr = notation.substr(endPos - 2, 2);
        chess::core::Position to = chess::core::Position::fromAlgebraic(destStr);

        // For piece moves (K, Q, R, B, N)
        if (pieceSymbol != ' ') {
            // Find the piece of this type and color that can move to the destination
            for (int rank = 0; rank < 8; rank++) {
                for (int file = 0; file < 8; file++) {
                    chess::core::Position from(file, rank);
                    chess::core::Piece* piece = board.getPiece(from);
                    
                    if (!piece || piece->getColor() != playerColor) {
                        continue;
                    }

                    if (getPieceSymbol(piece) != pieceSymbol) {
                        continue;
                    }

                    // Check if this piece can move to the destination
                    if (from == to) {
                        continue; // Can't move to the same square
                    }

                    chess::core::Piece* captured = board.getPiece(to);

                    auto builder = chess::core::Move::Builder(from, to, piece);
                    if (captured) {
                        builder.withCapture(captured);
                    }

                    auto move = builder.build();

                    // Basic check: piece should be able to move in that direction
                    // (More detailed validation happens in MoveValidator)
                    // For now, return the first valid-looking move
                    // TODO: Add proper move validation here
                    return std::make_unique<chess::core::Move>(move);
                }
            }
            return nullptr;
        }

        // For pawn moves (e.g., "e4", "exd5")
        // Check if there's a capture notation (x)
        size_t capturePos = notation.find('x');
        bool isCapture = (capturePos != std::string::npos && capturePos < endPos - 2);

        if (isCapture) {
            // Pawn capture notation: e.g., "exd5"
            // Extract the source file
            if (movePartStart >= capturePos) {
                return nullptr;
            }
            char fromFile = notation[movePartStart];
            if (fromFile < 'a' || fromFile > 'h') {
                return nullptr;
            }

            // Determine the rank to move from
            int destRank = to.getRank();
            int fromRank = destRank - (playerColor == chess::core::Color::WHITE ? -1 : 1);
            
            if (fromRank < 0 || fromRank > 7) {
                return nullptr;
            }

            chess::core::Position from(fromFile - 'a', fromRank);
            chess::core::Piece* piece = board.getPiece(from);
            
            if (!piece || piece->getColor() != playerColor || piece->getPieceName() != "Pawn") {
                return nullptr;
            }

            chess::core::Piece* captured = board.getPiece(to);
            if (!captured) {
                return nullptr; // Capture notation but no piece to capture
            }

            auto builder = chess::core::Move::Builder(from, to, piece);
            builder.withCapture(captured);
            
            return std::make_unique<chess::core::Move>(builder.build());
        } else {
            // Pawn move notation: e.g., "e4"
            // Find pawn on the same file that can move to this square
            int fromFile = to.getFile();
            int toRank = to.getRank();
            
            // For white pawns: they move UP (rank increases from 1 to 8)
            // For black pawns: they move DOWN (rank decreases from 6 to 1)
            
            // Check if there's a pawn one square before the destination
            // Try rank-1 for white (lower rank), rank+1 for black (higher rank)
            std::vector<int> candidateRanks;
            if (playerColor == chess::core::Color::WHITE) {
                if (toRank > 0) candidateRanks.push_back(toRank - 1);  // One square back
                if (toRank > 1) candidateRanks.push_back(toRank - 2);  // Two squares back (initial)
            } else {
                if (toRank < 7) candidateRanks.push_back(toRank + 1);  // One square back
                if (toRank < 6) candidateRanks.push_back(toRank + 2);  // Two squares back (initial)
            }
            
            for (int candidateRank : candidateRanks) {
                chess::core::Position from(fromFile, candidateRank);
                chess::core::Piece* piece = board.getPiece(from);
                
                if (!piece || piece->getColor() != playerColor || piece->getPieceName() != "Pawn") {
                    continue;
                }
                
                // For two-square moves, verify it's from the starting rank and path is clear
                if (std::abs(candidateRank - toRank) == 2) {
                    bool isStartingRank = (playerColor == chess::core::Color::WHITE && candidateRank == 1) ||
                                          (playerColor == chess::core::Color::BLACK && candidateRank == 6);
                    if (!isStartingRank) {
                        continue;  // Pawn can only move 2 squares from starting position
                    }
                    
                    // Check intermediate square
                    int intermRank = (candidateRank + toRank) / 2;
                    chess::core::Position intermediate(fromFile, intermRank);
                    if (board.getPiece(intermediate)) {
                        continue;  // Path is blocked
                    }
                }
                
                chess::core::Piece* captured = board.getPiece(to);
                auto builder = chess::core::Move::Builder(from, to, piece);
                if (captured) {
                    builder.withCapture(captured);
                }
                
                return std::make_unique<chess::core::Move>(builder.build());
            }
            
            return nullptr;
        }
    } catch (...) {
        return nullptr;
    }

    return nullptr;
}

}  // namespace chess::util
