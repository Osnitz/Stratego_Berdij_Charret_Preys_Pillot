#include "server.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "state.h"
using namespace state;
using namespace server;
/*int main() {
    auto server= new Server(8080, false);

    server->start();

    // Accepter des connexions et envoyer des messages
    std::thread serverThread([&]() {
        while (true) {
            server->acceptClients();

            // Envoyer un message de bienvenue à tous les clients
            std::string welcomeMessage = R"({"status":"connected","message":"Welcome to the server!"})";
            std::string specialMessage = R"({"mtf!"})";
            server->broadcastData(welcomeMessage);
            server->sendData(server->clients[0], specialMessage);


        }
    });

    // Arrêter le serveur après 60 secondes (pour tester)
    std::this_thread::sleep_for(std::chrono::seconds(60));
    server->stop();

    // Attendre que le thread termine
    serverThread.join();

    return 0;
}*/

int main() {
    auto game = new Game();
    auto currentPlayer= game->getCurrentPlayer();
    std::string filepath ="/home/matthieu/CLionProjects/Stratego_Berdij_Charret_Preys_Pillot/src/shared/state/config/Balance.csv";
    game->loadConfig(filepath);
    game->switchTurn();
    game->loadConfig(filepath);
    game->switchTurn();

    auto server= new Server(8080, false,game);
    server->start();
    while (server->clients.size() < 2) {
        server->acceptClients();
    }
    try {
        server->sendIdentifierToClients();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::thread serverThread([&]() {
        //while (true) {
            // Exemple : Envoyer une requête à un client spécifique
            int clientId = currentPlayer->getPlayerID(); // Exemple d'identifiant client
            //server->sendRequestToClient(clientId, R"({"action":"move","message":"Your turn"})");

            // Attendre la réponse
            //std::string response = server->waitForResponseFromClient(clientId);
            //std::cout << "Received from client " << clientId << ": " << response << std::endl;
            //game->switchTurn();
            //currentPlayer = game->getCurrentPlayer();
            auto gameState = server->serializeGameState();
            server->broadcastData(gameState);
        //}
    });

    std::this_thread::sleep_for(std::chrono::seconds(60));
    server->stop();
    serverThread.join();

    return 0;
}

