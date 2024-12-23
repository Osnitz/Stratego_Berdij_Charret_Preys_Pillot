//
// Created by matthieu on 16/12/24.
//
#include <iostream>
#include <random>

#include "PlayerController.h"
#include "client.h"


using namespace client;


AIController::AIController(engine::Engine* eng, ai::AIInterface* aiModule)
    : PlayerController(eng, aiModule)
{
    engine=eng;
    this->aiModule = aiModule;
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


void AIController::handlePlacement(state::Game* game)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 3);

    std::string filePath;
    int randomConfig= distr(gen);
    switch (randomConfig)
    {
    case 1:
        filePath = "../src/shared/state/config/Offensive.csv";
        break;
    case 2:
        filePath = "../src/shared/state/config/Defensive.csv";
        break;
    case 3:
        filePath = "../src/shared/state/config/Balance.csv";
        break;
    default:
        std::cerr << "Invalid choice. Please choose a configuration (1: Offensive, 2:Defensive, 3:Balance): " <<
            std::endl;
    }
    engine->handleCmdPlacement(filePath);
}

engine::Engine* AIController::getEngine()
{
    return engine;
}

ai::AIInterface* AIController::getAiModule()
{
    return aiModule;
}

AIController::~AIController()
{
    delete aiModule;
    delete engine;
}