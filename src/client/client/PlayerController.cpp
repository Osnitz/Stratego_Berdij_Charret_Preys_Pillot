//
// Created by matthieu on 11/12/24.
//

#include "PlayerController.h"

#include <stdexcept>

using namespace client;

PlayerController::PlayerController(engine::Engine * eng, ai::AIInterface* aiModule)
{
    engine = eng;
    this->aiModule = aiModule;
}

PlayerController::~PlayerController()
{
    delete aiModule;
    delete engine;
}

std::string PlayerController::getProjectRootDirectory() {
    // Full path to the current file
    std::string filePath = __FILE__;

    // Find the "Stratego" directory in the path
    std::size_t pos = filePath.find("Stratego_Berdij_Charret_Preys_Pillot");
    if (pos == std::string::npos) {
        throw std::runtime_error("Unable to find 'Stratego_Berdij_Charret_Preys_Pillot' in the path");
    }

    // Extract the path up to and including "Stratego"
    return filePath.substr(0, pos + std::string("Stratego_Berdij_Charret_Preys_Pillot").length());
}

std::string PlayerController::constructPath(const std::string& relativePath) {
    return getProjectRootDirectory() + "/" + relativePath;
}

