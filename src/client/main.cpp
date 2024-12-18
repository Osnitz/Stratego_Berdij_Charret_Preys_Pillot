// The following lines are here to check that SFML is installed and working
#include <iostream>
#include <client/PlayerController.h>
#include <SFML/Graphics.hpp>

void testSFML()
{
    sf::Texture texture;
}

// end of test SFML

#include <state.h>
#include <client.h>


using namespace std;
using namespace state;

/*int main() {
    auto game = Game::getInstance();
    game->startGame();
}*/

int main()
{

    Game* game = new Game();
    std::vector<Player*> players = {game->getPlayer1(), game->getPlayer2()};
    state::Player* currentPlayer;
    client::PlayerController* playerController;
    std::vector<int> coords;
    std::pair<int, int> from;
    std::pair<int, int> to;

    auto gameEngine = new engine::Engine(game, players);
    auto scenarioManager = new client::ScenarioManager(gameEngine);

    auto gameMode = scenarioManager->getScenarioChoice();
    scenarioManager->setMode(gameMode);

    scenarioManager->initializeControllers();

    gameEngine->startGame();

    for (size_t i = 0; i < players.size(); i++)
    {
        currentPlayer = game->currentPlayer;
        playerController = scenarioManager->getPlayerController(currentPlayer);
        playerController->handlePlacement(game);
    }


    int test = 0;
    while (test < 100)
    {
        test++;
        currentPlayer = game->getCurrentPlayer();
        game->displayBoard(*currentPlayer);
        playerController = scenarioManager->getPlayerController(currentPlayer);
        coords = playerController->getPlayerInput();
        from = std::make_pair(coords[0], coords[1]);
        to = std::make_pair(coords[2], coords[3]);
        if (!playerController->executeCmd(from, to, game->getCurrentPlayer()))
        {
            continue;
        }
    }
    std::cout<<"Nb de tours"<<test<<std::endl;

    delete currentPlayer;
    delete playerController;
    delete gameEngine;
    delete scenarioManager;
    delete game;
    return 0;
}
