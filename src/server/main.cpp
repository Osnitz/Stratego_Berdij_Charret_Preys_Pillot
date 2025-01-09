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
    // Initialisez le jeu (placeholder pour votre logique de jeu)
    Game* game = new Game();
    auto gameEngine = new Engine(game);
    auto scenarioManager = new ScenarioManager(gameEngine);

    client::PlayerController* playerController;
    auto gameMode = scenarioManager->getScenarioChoice();
    scenarioManager->setMode(gameMode);
    scenarioManager->initializeControllers();
    Player* currentPlayer;
    for (size_t i = 0; i < 2; i++)
    {
        currentPlayer = game->getCurrentPlayer();
        playerController = scenarioManager->getPlayerController(currentPlayer);
        playerController->handlePlacement(game);
    }
    std::string filepath =
        "/home/matthieu/CLionProjects/Stratego_Berdij_Charret_Preys_Pillot/src/shared/state/config/Balance.csv";
    game->loadConfig(filepath);
    game->switchTurn();
    game->loadConfig(filepath);
    game->switchTurn();
    currentPlayer = game->getCurrentPlayer();

    int clientId;

    // Créez le serveur
    auto server = new Server(8080, true, game);
    server->start();

    // Attendez deux clients
    std::cout << "Waiting for clients..." << std::endl;
    while (server->clients.size() < 2)
    {
        server->acceptClients();
    }

    server->sendIdentifierToClients();
    ServerRequest moveRequest;
    moveRequest.type = server::RequestType::Move;
    std::thread serverThread([&]()
    {
        int index = 0;
        while (index < 10)
        {

            auto gameState = server->serializeGameState();
            for (int client_fd : server->clients)
            {
                server->sendLargeJson(client_fd, gameState);
            }

            std::cout << "game state bien reçu par tout les clients" << std::endl;

            // Envoyez une requête de déplacement (Move)*/
            clientId = currentPlayer->getPlayerID();
            server->sendRequestToClient(server->clients[clientId], moveRequest);
            // Recevez la réponse du client
            Json::Value moveResponse = server->receiveResponseFromClient(server->clients[clientId]);
            auto coords = server->handleClientResponse(server->clients[clientId], moveResponse);
            playerController = scenarioManager->getPlayerController(currentPlayer);
            playerController->executeCmd(std::make_pair(coords[0], coords[1]),
                std::make_pair(coords[2], coords[3]), currentPlayer);
            index++;
            currentPlayer = game->getCurrentPlayer();
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(600));
    server->stop();
    serverThread.join();
    return 0;
}
