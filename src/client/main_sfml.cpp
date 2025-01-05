#include <iostream>
#include <SFML/Graphics.hpp>
#include "state/Game.h"
#include "engine/Engine.h"
#include "client/ScenarioManager.h"
#include "client/Render.h"

using namespace state;
using namespace engine;
using namespace client;

int main() {
    Game* game = new Game();
    Engine* engine = new Engine(game);
    ScenarioManager* scenarioManager = new ScenarioManager(engine);
    Render render(engine, scenarioManager);

    bool gameOver = false;

    while (!gameOver) {
        auto currentPlayer = game->getCurrentPlayer();
        auto currentController = scenarioManager->getPlayerController(currentPlayer);

        // Vérification indirecte si le joueur est contrôlé par une IA
        auto coords = currentController->getPlayerInput();
        if (!coords.empty()) { // Suppose que getPlayerInput retourne des coordonnées uniquement pour une IA
            std::pair<int, int> from = {coords[0], coords[1]};
            std::pair<int, int> to = {coords[2], coords[3]};
            currentController->executeCmd(from, to, currentPlayer);
        } else {
            // Si aucune coordonnée n'est donnée, on considère que c'est un joueur humain
            render.run();
        }

        // Vérification de la condition de victoire
        if (engine->checkWin() != WinCondition::None) {
            gameOver = true;
        }
    }

    return 0;
}
