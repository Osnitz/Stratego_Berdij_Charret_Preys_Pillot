//
// Created by matthieu on 16/12/24.
//

#include <iostream>

#include "PlayerController.h"
#include "client.h"


using namespace client;


HumanController::HumanController(engine::Engine* eng, int playerID, ai::AIInterface* aiModule)
    : PlayerController(eng, aiModule, playerID)
{
    engine = eng;
    this->aiModule = aiModule;
    playerIdx = playerID;
}


bool HumanController::isAI()
{
    return (aiModule != nullptr);
}

std::vector<int> HumanController::getPlayerInput()
{
    std::vector<int> coords;
    int startX, startY, endX, endY;

    std::cout << "Player " << playerIdx << ", enter the coordinates of the piece to move (e.g. '2 3'): \n";
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
  //  return playerID;
}

HumanController::~HumanController()
{
    delete aiModule;
    delete engine;
}

void HumanController::handlePlacement(state::Game* game)
{
    auto currentPlayer = game->getCurrentPlayer();
    std::cout << "Player " << currentPlayer->getPlayerID() << ", choose your configuration " << std::endl;
    std::cout << "(1: Offensive, 2:Defensive, 3:Balance): " << std::endl;
    int choice;
    std::cin >> choice;
    std::string filePath;
    switch (choice)
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
