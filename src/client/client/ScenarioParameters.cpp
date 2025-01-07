//
// Created by matthieu on 07/01/25.
//
#include "ScenarioParameters.h"
using namespace client;
ScenarioParameters::ScenarioParameters(GameMode mode, ai::AIInterface* aiModule0,
                                       ai::AIInterface* aiModule1): mode(mode), aiModule0(aiModule0), aiModule1(aiModule1){
}

ScenarioParameters::~ScenarioParameters() = default;