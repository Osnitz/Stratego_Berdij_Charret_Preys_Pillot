#include <iostream>
//
// Created by matthieu on 08/01/25.
//
#include <fstream>

#include "client.h"
#include <thread>
using namespace client;

int main()
{
    auto game = new state::Game();
    auto client = new NetworkClient("127.0.0.1", 8080, game);
    client->connectToServer();
    std::string gamestate = "NULL";
    //client->monitorServerConnection();
    //client->handleServerRequest();
    int id = client->receiveIdentifier();
    client->setPlayerId(id);
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
    std::cout << "JSON saved to " << "output.json" << std::endl;*/
    auto pieces = client->deserializeGameState(gamestate);
    std::cout << "deserilize done\n" << std::endl;
    client->updateGameState(pieces);
    game->displayBoard(*game->getPlayerByID(client->getPlayerID()));

    return 0;
}
