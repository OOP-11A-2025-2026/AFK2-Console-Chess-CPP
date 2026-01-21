#include "GameController.hpp"

#include <stdexcept>

namespace chess::engine {

GameController::GameController()
    : game(nullptr),
      undoManager(std::make_unique<rules::UndoManager>()) {}

void GameController::newGame(const core::Player& white,
                             const core::Player& black) {
    game = std::make_unique<core::Game>(white, black);
    undoManager->clear();
}

void GameController::loadGame(const std::string& pgnFile) {
    if (pgnFile.empty())
        throw std::invalid_argument("PGN file path is empty");

    PgnParser parser;
    auto parsedGame = parser.parse(pgnFile);

    game = std::make_unique<core::Game>(
        parsedGame.whitePlayer,
        parsedGame.blackPlayer
    );

    undoManager->clear();

    for (const auto& move : parsedGame.moves) {
        undoManager->saveSnapshot(*game);
        game->applyMove(move);
    }
}

void GameController::saveGame(const std::string& pgnFile) const {
    ensureGameExists();
    if (pgnFile.empty())
        throw std::invalid_argument("PGN file path is empty");

    PgnWriter writer;
    writer.write(pgnFile, *game);
}

void GameController::makeMove(const core::Move& move) {
    ensureGameExists();

    undoManager->saveSnapshot(*game);
    game->applyMove(move);
    updateGameState();

    if (botEnabled)
        makeBotMove();
}

void GameController::makeMoveAlgebraic(const std::string& notation) {
    ensureGameExists();
    if (notation.empty())
        throw std::invalid_argument("Move notation is empty");

    undoManager->saveSnapshot(*game);
    game->applyMoveAlgebraic(notation);
    updateGameState();

    if (botEnabled)
        makeBotMove();
}

void GameController::undo() {
    ensureGameExists();
    undoManager->undo(*game);
}

void GameController::offerDraw() {
    ensureGameExists();
    game->offerDraw();
}

void GameController::acceptDraw() {
    ensureGameExists();
    game->acceptDraw();
}

void GameController::resign(core::Color resigningColor) {
    ensureGameExists();
    game->resign(resigningColor);
}

void GameController::enableBot(std::unique_ptr<ChessEngine> engine) {
    if (!engine)
        throw std::invalid_argument("Bot engine is null");

    botEngine = std::move(engine);
    botEnabled = true;
}

void GameController::disableBot() {
    botEngine.reset();
    botEnabled = false;
}

void GameController::makeBotMove() {
    if (!botEnabled || !botEngine)
        return;

    core::Move bestMove = botEngine->getBestMove(*game);
    undoManager->saveSnapshot(*game);
    game->applyMove(bestMove);
    updateGameState();
}

const core::Game& GameController::getGame() const {
    ensureGameExists();
    return *game;
}

core::GameState GameController::getGameState() const {
    ensureGameExists();
    return game->getState();
}

void GameController::ensureGameExists() const {
    if (!game)
        throw std::runtime_error("No active game");
}

void GameController::updateGameState() {
    game->updateState();
}

}
