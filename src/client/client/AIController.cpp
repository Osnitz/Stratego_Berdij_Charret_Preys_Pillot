//
// Created by matthieu on 16/12/24.
//
#include <iostream>

#include "PlayerController.h"
#include "client.h"


using namespace client;


AIController::AIController(engine::Engine* eng, int id, ai::AIInterface* aiModule)
    : PlayerController(eng, id, aiModule){}


bool AIController::isAI()
{
    return (aiModule != nullptr);
}

std::vector<int> AIController::getPlayerInput()
{
    std::vector<int> coords;
    int startX, startY, endX, endY;
    auto positions = aiModule->calculateMove(engine->getGame());

    startX = positions[0];
    startY = positions[1];
    endX = positions[2];
    endY = positions[3];

    coords.push_back(startX);
    coords.push_back(startY);
    coords.push_back(endX);
    coords.push_back(endY);
    return coords;
}

bool AIController::executeCmd(std::pair<int, int> from, std::pair<int, int> to, state::Player* currentPlayer)
{
    // Transmet la commande à l’Engine
    bool success = engine->handleCmdMove(from, to);
    return success;
}

int AIController::getPlayerID()
{
    return playerID;
}

AIController::~AIController()
{
    delete aiModule;
    delete engine;
}