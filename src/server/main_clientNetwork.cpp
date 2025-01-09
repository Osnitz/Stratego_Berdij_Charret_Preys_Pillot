#include <iostream>
//
// Created by matthieu on 08/01/25.
//
#include <fstream>

#include "server.h"
#include <thread>
#include <state/Player.h>
using namespace server;

int main()
{
    try
    {
        auto game = new state::Game();
        auto client = new NetworkClient("192.168.32.234", 8080, game);
        client->connectToServer();
        auto id = client->receiveIdentifier();
        client->setPlayerID(id);

        auto myPlayer = game->getPlayerByID(client->getPlayerID());
        // Boucle principale pour recevoir et traiter les requêtes
        while (true)
        {
            auto gameStateString = client->receiveLargeJson();
            auto gameState = client->deserializeGameState(gameStateString);
            client->updateGameState(gameState);
            //std::cout << "Game state updated!" << std::endl;
            game->displayBoard(*myPlayer);

            if (game->getCurrentPlayer()->getPlayerID() == client->getPlayerID())
            {
                // Recevez une requête du serveur
                server::ServerRequest request = client->receiveRequest();
                // Traitez la requête (demandera des inputs utilisateur)
                client->handleServerRequest(request);
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
