#include <iostream>

// The following lines are here to check that SFML is installed and working
#include <client/PlayerController.h>
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}
// end of test SFML

#include <state.h>
#include <engine.h>
#include <client.h>


using namespace std;
using namespace state;

/*int main() {
    auto game = Game::getInstance();
    game->startGame();
}*/

int main() {

    Game* game = new Game();
    std::vector<Player*> players = {game->getPlayer1(), game->getPlayer2()};

    auto gameEngine = new engine::Engine(game, players);

    auto scenarioManager = new client::ScenarioManager(gameEngine);

    auto gameMode = scenarioManager->getScenarioChoice();
    scenarioManager->setMode(gameMode);

    scenarioManager->initializeControllers();

    gameEngine->startGame();

    scenarioManager->handlePlacement(game);


    int test =0;
    while (test<100)
    {
        test++;
        auto currentPlayer = game->getCurrentPlayer();
        game->displayBoard(*currentPlayer);
        auto playerController = scenarioManager->getPlayerController(currentPlayer);
        auto coords = playerController->getPlayerInput();
        auto from = std::make_pair(coords[0], coords[1]);
        auto to = std::make_pair(coords[2], coords[3]);
        if (!playerController->executeCmd(from, to, game->getCurrentPlayer()))
        {
            continue;
        }
    }

    return 0;}
