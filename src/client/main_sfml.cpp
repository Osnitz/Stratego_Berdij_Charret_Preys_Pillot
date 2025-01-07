//
// Created by matthieu on 06/01/25.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "client.h"
#include "state.h"


using namespace client;
using namespace state;
using namespace engine;


int main() {
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

    std::cout << "Game started. Entering Placement Phase." << std::endl;

    for (size_t i = 0; i < 2; i++)
    {
        currentPlayer = game->getCurrentPlayer();
        playerController = scenarioManager->getPlayerController(currentPlayer);
        playerController->handlePlacement(game);
    }

    Render render(60, game); // Fenêtre 800x600, cases de 60 pixels
    auto window = render.getWindow();

    while (window->isOpen())
    {
        render.handleEvents();
        window->clear();
        render.drawBoard();
        render.drawGrid();
        render.drawCoordinates();
        render.drawPiecesOnBoard(game);
        window->display();

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

        if (playerController->isAI())
        {
            coords = playerController->getPlayerInput();
        }
        else
        {
            coords = render.getPlayerInput();
        }

        if (coords[0] == -1 && coords[1] == -1) {
            std::cout << "Jeu interrompu par l'utilisateur." << std::endl;
            break;
        }

        from = std::make_pair(coords[0], coords[1]);
        to = std::make_pair(coords[2], coords[3]);
        if (!playerController->executeCmd(from, to, game->getCurrentPlayer()))
        {
            continue;
        }


    }
    return 0;
}
