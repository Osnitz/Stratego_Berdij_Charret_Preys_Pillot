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
#include "engine/WinCondition.h"

using namespace std;
using namespace state;
using namespace engine;

int main()
{
    Game* game = new Game();
    Player* currentPlayer;
    client::PlayerController* playerController;
    std::vector<int> coords;
    std::pair<int, int> from;
    std::pair<int, int> to;

    auto gameEngine = new engine::Engine(game);
    auto scenarioManager = new client::ScenarioManager(gameEngine);

    auto gameMode = scenarioManager->getScenarioChoice();
    scenarioManager->setMode(gameMode);

    scenarioManager->initializeControllers();

    std::cout << "Game started. Entering Placement Phase." << endl;

    for (size_t i = 0; i < 2; i++)
    {
        currentPlayer = game->getCurrentPlayer();
        playerController = scenarioManager->getPlayerController(currentPlayer);
        playerController->handlePlacement(game);
    }


    int test = 0;
    while (true)
    {
        test++;
        currentPlayer = game->getCurrentPlayer();
        game->displayBoard(*currentPlayer);
        playerController = scenarioManager->getPlayerController(currentPlayer);

        WinCondition winCondition = gameEngine->checkWin();
        if (winCondition != None)
        {
            if (winCondition == FlagCaptured)
            {
                std::cout << "Flag has been captured. Game over." << std::endl;
            }
            else if (winCondition == NoValidMoves)
            {
                std::cout << "Player " << currentPlayer->getPlayerID() << " has no valid moves left. Game over." << std::endl;
                std::cout << "Player " << game->getOpponent()->getPlayerID() << " wins!" << std::endl;
            }
            break;
        }

        coords = playerController->getPlayerInput();
        from = std::make_pair(coords[0], coords[1]);
        to = std::make_pair(coords[2], coords[3]);
        if (!playerController->executeCmd(from, to, game->getCurrentPlayer()))
        {
            continue;
        }
    }

    std::cout << "Nb de tours : " << test << std::endl;

    delete scenarioManager;
    delete gameEngine;
    delete game;

    return 0;
}