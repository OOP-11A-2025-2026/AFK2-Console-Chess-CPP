# AFK2 Console Chess (C++)

A small, object-oriented console chess project in C++17. It includes core move validation, a simple clock, and PGN save/load helpers. The executable currently runs a suite of sanity checks (no interactive UI yet).

## Table of Contents
- Features
- Requirements
- Installation
- Usage
- Project Structure
- How It Works
- Architecture
- Commands
- Contributing
- License

## Features

### Core Gameplay
- Piece movement validation for standard moves (basic checks against leaving king in check).
- Move notation helpers (coordinate and basic algebraic notation).
- Game state tracking (whose turn, simple states like ongoing/draw/checkmate marker).
- Chess clock support (per-player timers, default 5 minutes).

### Game Management
- Save & Load: PGN file support (parses common SAN, casts minimal tags, replays moves to restore turn).
- Move History: Records moves played.
- Draw Offers: Offer/accept/decline hooks in `Game`.

### Design Highlights
- Clean OOP core (Board as single source of truth; Piece hierarchy with copy semantics).
- Modular helpers (notation, PGN, validation) separated from core models.

> Note: Special moves like castling/en passant/promotion logic are minimal/incomplete in the current build. Use the included tests/PGN round-trip as the supported path.

## Requirements
- C++17 compiler (clang++ or g++).
- make.
- GoogleTest available if you want to run `make test`.

## Installation
1) Clone or download the repo.
2) Ensure your compiler and make are on PATH.

## Usage

### Quick Start
- Build: `make build`
- Run console harness (sanity tests + PGN round-trip): `make run`
- Run unit tests (if gtest installed): `make test`

### Example Session (`make run`)
- Runs a suite of inline tests (color/position/move/board/pieces/player/clock/game/notation/validator).
- Executes a PGN save/load round-trip and writes `sample_game.pgn` in the project root.

## Project Structure
- `src/chess/core/` — Board, Piece, Move, Player, Game, ChessClock, Position, Color, GameState.
- `src/chess/pieces/` — Pawn, Knight, Bishop, Rook, Queen, King.
- `src/chess/rules/` — MoveValidator (basic legality + king safety checks).
- `src/chess/util/` — AlgebraicNotationUtil (to/from long and simple algebraic).
- `src/chess/pgn/` — PgnIO (load/save PGN, apply moves).
- `src/main.cpp` — Console harness running sanity checks and PGN demo.
- `tests/` — GoogleTest scaffolding (placeholders).

## How It Works
1) Board initializes standard starting position.
2) Moves are built via `Move::Builder` with flags for capture/castling/en-passant/promotion (special rules partly stubbed).
3) `MoveValidator` checks piece ownership, pseudo-legal destinations, and king safety via board copy.
4) `AlgebraicNotationUtil` converts moves to/from simple algebraic/coordinate notation.
5) `PgnIO` replays SAN tokens from PGN into a `Game`, updating move history and turn; saving rebuilds from history on a fresh board.
6) `ChessClock` tracks time per color; `Game` switches turns and clears draw offers.

## Architecture
- **Board as source of truth**: holds pieces and king positions.
- **Piece hierarchy**: virtual `getLegalDestinations` plus `copy` to deep-copy board state.
- **Thin orchestrator**: `Game` owns board/players/clock, toggles turns, and stores history.
- **Utilities**: Notation and PGN are layered on top without mutating core rules.

## Commands
- `make build` — Compile the project.
- `make run` — Build then run the console harness (sanity tests + PGN demo).
- `make test` — Build and run unit tests (requires gtest).
- `make clean` — Remove build artifacts.
- `make rebuild` — Clean then build.
- `make debug` — Debug build with symbols.
- `make help` — List Makefile targets.

## Contributing
- Keep changes modular (core vs. rules vs. utilities).
- Add/extend tests where possible (gtest suite or inline harness).
- Document new command-line or PGN behaviors in this README.

## License
See `LICENSE` in the project root.
