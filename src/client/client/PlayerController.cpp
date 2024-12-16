//
// Created by matthieu on 11/12/24.
//

#include "PlayerController.h"

using namespace client;

PlayerController::PlayerController(engine::Engine * eng , int id, ai::AIInterface* aiModule)
{
    engine = eng;
    playerID = id;
    this->aiModule = aiModule;
}

PlayerController::~PlayerController()
{
    delete aiModule;
    delete engine;
}

