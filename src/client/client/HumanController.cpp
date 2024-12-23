//
// Created by matthieu on 16/12/24.
//

#include <iostream>

#include "PlayerController.h"
#include "client.h"


using namespace client;

std::string getProjectRootDirectory() {
    // Full path to the current file
    std::string filePath = __FILE__;

    // Find the "Stratego" directory in the path
    std::size_t pos = filePath.find("Stratego");
    if (pos == std::string::npos) {
        throw std::runtime_error("Unable to find 'Stratego' in the path");
    }

    // Extract the path up to and including "Stratego"
    return filePath.substr(0, pos + std::string("Stratego").length());
}

std::string constructPath(const std::string& relativePath) {
    return getProjectRootDirectory() + "/" + relativePath;
}

HumanController::HumanController(engine::Engine* eng, ai::AIInterface* aiModule)
    : PlayerController(eng, aiModule)
{
    engine = eng;
    this->aiModule = aiModule;
}


std::vector<int> HumanController::getPlayerInput()
{
    std::vector<int> coords;
    int startX, startY, endX, endY;

    std::cout << "Player " << engine->getGame()->currentPlayer->getPlayerID() << ", enter the coordinates of the piece to move (e.g. '2 3'): \n";
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


HumanController::~HumanController()
{
    delete aiModule;
}

void HumanController::handlePlacement(state::Game* game)
{
    auto currentPlayer = game->getCurrentPlayer();
    std::cout << "Player " << currentPlayer->getPlayerID() << ", choose your configuration " << std::endl;
    std::cout << "(1: Offensive, 2:Defensive, 3:Balance): " << std::endl;
    int choice;
    std::cin >> choice;
    std::string filePath;

    while (choice < 1 || choice > 3)
    {
        std::cerr << "Invalid choice. Please choose a configuration (1: Offensive, 2:Defensive, 3:Balance): " <<std::endl;
        std::cin >> choice;
    }
    switch (choice)
    {
    case 1:
        filePath = constructPath("src/shared/state/config/Offensive.csv");
        break;
    case 2:
        filePath = constructPath("src/shared/state/config/Defensive.csv");
        break;
    case 3:
        filePath = constructPath("src/shared/state/config/Balance.csv");
        break;
    }
    engine->handleCmdPlacement(filePath);
}

engine::Engine* HumanController::getEngine()
{
    return engine;
}

ai::AIInterface* HumanController::getAiModule()
{
    return aiModule;
}

