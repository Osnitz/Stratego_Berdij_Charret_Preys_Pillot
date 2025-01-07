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
    Render render(60, game); // Fenêtre 800x600, cases de 60 pixels
    auto window = render.getWindow();

    Player* currentPlayer;
    PlayerController* playerController;
    std::vector<int> coords;
    std::pair<int, int> from;
    std::pair<int, int> to;

    auto gameEngine = new engine::Engine(game);
    auto scenarioManager = new client::ScenarioManager(gameEngine);

    auto scenarioParameters = render.displayInitializationScreen();
    scenarioManager->setMode(scenarioParameters.mode);
    scenarioManager->setAiModules(scenarioParameters.aiModule0, scenarioParameters.aiModule1);

    scenarioManager->initializeControllers();


    for (size_t i = 0; i < 2; i++)
    {
        currentPlayer = game->getCurrentPlayer();
        playerController = scenarioManager->getPlayerController(currentPlayer);
        if (playerController->isAI())
        {
            playerController->handlePlacement(game);
        }
        else
        {
            auto filepath =render.displayConfigurationSelection("Choix de la configuration du joueur " +
                std::to_string(currentPlayer->getPlayerID()));
            gameEngine->handleCmdPlacement(filepath);
        }

    }


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
        if (winCondition != None) {
            int winnerPlayerID = (winCondition == FlagCaptured || winCondition == NoValidMoves)
                                     ? game->getOpponent()->getPlayerID()
                                     : game->getCurrentPlayer()->getPlayerID();

            render.displayEndScreen(winnerPlayerID);
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
