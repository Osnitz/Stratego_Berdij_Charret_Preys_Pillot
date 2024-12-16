//
// Created by matthieu on 16/12/24.
//

#include <iostream>

#include "PlayerController.h"
#include "client.h"


using namespace client;


HumanController::HumanController(engine::Engine* eng, int id, ai::AIInterface* aiModule)
    : PlayerController(eng, id, aiModule)
{
    engine = eng;
    playerID = id;
    this->aiModule = aiModule;
}


bool HumanController::isAI()
{
    return (aiModule != nullptr);
}

std::vector<int> HumanController::getPlayerInput()
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

bool HumanController::executeCmd(std::pair<int, int> from, std::pair<int, int> to, state::Player* currentPlayer)
{
    // Transmet la commande à l’Engine
    bool success = engine->handleCmdMove(from, to);
    return success;
}

int HumanController::getPlayerID()
{
    return playerID;
}

HumanController::~HumanController()
{
    delete aiModule;
    delete engine;
}