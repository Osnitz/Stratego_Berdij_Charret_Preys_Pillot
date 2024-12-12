//
// Created by matthieu on 11/12/24.
//

#include <iostream>

#include "PlayerController.h"
#include "client.h"
#include "state.h"


using namespace client;


PlayerController::PlayerController(engine::Engine* eng, int id)
    : engine(eng), playerID(id), aiModule(nullptr)
{
}

void PlayerController::switchToAI(ai::AIInterface* newAI)
{
    aiModule = newAI; // pointeur valide vers une IA
}

void PlayerController::switchToHuman()
{
    aiModule = nullptr; // plus d'IA
}

bool PlayerController::isAI()
{
    return (aiModule != nullptr);
}

std::vector<int> PlayerController::getUserInput()
{
    std::vector<int> coords;
    int startX, startY, endX, endY;

    std::cout << "Player " << playerID << ", enter the coordinates of the piece to move (e.g. '2 3'): \n";
    std::cin >> startX >> startY;
    coords.push_back(startX);
    coords.push_back(startY);

    std::cout << "Enter the destination coordinates (e.g. '2 4'): \n";
    std::cin >> endX >> endY;
    coords.push_back(endX);
    coords.push_back(endY);
    return coords;
}

bool PlayerController::executeCmd(std::pair<int, int> from, std::pair<int, int> to, state::Player* currentPlayer)
{
    // Transmet la commande à l’Engine
    bool success = engine->handleCmdMove(from, to);
    return success;
}

int PlayerController::getPlayerID()
{
    return playerID;
}
