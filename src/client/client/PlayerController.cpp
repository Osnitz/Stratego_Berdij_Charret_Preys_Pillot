//
// Created by matthieu on 11/12/24.
//

#include "PlayerController.h"

using namespace client;

PlayerController::PlayerController(engine::Engine * eng, ai::AIInterface* aiModule, int playerID)
{
    engine = eng;
    this->aiModule = aiModule;
    playerIdx = playerID;
}

PlayerController::~PlayerController()
{
    delete aiModule;
    delete engine;
}

