#include "server.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "state.h"
#include "engine.h"
#include "../client/client.h"
using namespace state;
using namespace server;
using namespace engine;
using namespace client;


int main()
{
    Game* game = new Game();

    // Créez le serveur
    auto server = new Server(8080, true, game);
    server->start();

    std::cout << "Waiting for clients..." << std::endl;
    while (server->clients.size() < 2)
    {
        server->acceptClients();
    }

    server->sendIdentifierToClients();
    ServerRequest moveRequest;
    moveRequest.type = server::RequestType::Move;

    auto gameEngine = new Engine(game);
    auto scenarioManager = new ScenarioManager(gameEngine);


    client::PlayerController* playerController;
    auto gameMode = GameMode::PVP; //scenarioManager->getScenarioChoice();
    scenarioManager->setMode(gameMode);
    scenarioManager->initializeControllers();
    Player* currentPlayer;

  server->handlePlacement(gameEngine);


    int clientId;

    std::thread serverThread([&]()
    {
        while (true)
        {
            currentPlayer = game->getCurrentPlayer();
            auto gameState = server->serializeGameState();
            for (int client_fd : server->clients)
            {
                server->sendLargeJson(client_fd, gameState);
            }

            //std::cout << "game state bien reçu par tout les clients" << std::endl;
            WinCondition winCondition = gameEngine->checkWin();
            if (winCondition != None)
            {
                if (winCondition == FlagCaptured)
                {
                    std::cout << "Flag has been captured. Game over." << std::endl;

                }
                else if (winCondition == NoValidMoves)
                {
                    std::cout << "Player " << currentPlayer->getPlayerID() << " has no valid moves left. Game over." <<
                        std::endl;
                    std::cout << "Player " << game->getOpponent()->getPlayerID() << " wins!" << std::endl;
                }
                break;
            }

            // Envoyez une requête de déplacement (Move)*/
            clientId = currentPlayer->getPlayerID();
            server->sendRequestToClient(server->clients[clientId], moveRequest);

            Json::Value moveResponse = server->receiveResponseFromClient(server->clients[clientId]);

            auto coords = server->handleClientResponse(server->clients[clientId], moveResponse);

            playerController = scenarioManager->getPlayerController(currentPlayer);
            playerController->executeCmd(std::make_pair(coords[0], coords[1]),
                                         std::make_pair(coords[2], coords[3]), currentPlayer);
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(600));
    server->stop();
    serverThread.join();
    return 0;
}
