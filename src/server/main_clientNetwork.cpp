#include <iostream>
//
// Created by matthieu on 08/01/25.
//
#include <fstream>

#include "server.h"
#include <thread>
using namespace server;

/*int main()
{
    auto game = new state::Game();
    auto client = new NetworkClient("127.0.0.1", 8080, game);
    client->connectToServer();
    std::string gamestate = "NULL";
    //client->monitorServerConnection();
    //client->handleServerRequest();
    int id = client->receiveIdentifier();
    client->setPlayerID(id);
    std::cout<<"clientID :"<<client->getPlayerID()<<std::endl;

    gamestate = client->receiveGameStateWithDynamicBuffer();
    std::cout << "gamestate received\n" << std::endl;
    /*std::ofstream outFile("output.json");
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file: output.json");
    }

    // Écrit le contenu du JSON dans le fichier
    outFile << gamestate;

    // Ferme le fichier
    outFile.close();
    std::cout << "JSON saved to " << "output.json" << std::endl;
    auto pieces = client->deserializeGameState(gamestate);
    std::cout << "deserilize done\n" << std::endl;
    client->updateGameState(pieces);
    game->displayBoard(*game->getPlayerByID(client->getPlayerID()));

    return 0;
}*/

#include <iostream>

int main() {
    try {
        // Connectez-vous au serveur
        auto game = new state::Game();
        auto client = new NetworkClient("127.0.0.1", 8080, game);
        client->connectToServer();
        auto id = client->receiveIdentifier();


        client->setPlayerID(id);
        auto myPlayer = game->getPlayerByID(client->getPlayerID());
        // Boucle principale pour recevoir et traiter les requêtes
        while (true) {
                auto gameStateString = client->receiveLargeJson();
                auto gameState = client->deserializeGameState(gameStateString);
                client->updateGameState(gameState);
                game->displayBoard(*myPlayer);

                // Recevez une requête du serveur
                server::ServerRequest request = client->receiveRequest();
                // Traitez la requête (demandera des inputs utilisateur)
                client->handleServerRequest(request);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
