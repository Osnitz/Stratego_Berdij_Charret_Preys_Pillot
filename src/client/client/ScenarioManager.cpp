#include <iostream>
#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include <memory>
#include "client.h"
#include "ai.h"


namespace client {
    ScenarioManager::ScenarioManager(engine::Engine* eng) : engine(eng), mode(GameMode::PVP) {}

    // Configure le mode de jeu
    void ScenarioManager::setMode(GameMode m) {
        mode = m;
    }

    // Initialise les PlayerControllers en fonction du mode de jeu
    void ScenarioManager::initializeControllers() {
        // PlayerController prend un pointeur vers Engine et un playerID
        auto player1Controller = new client::PlayerController(engine, 0);
        auto player2Controller = new client::PlayerController(engine, 1);

        playerControllers.push_back(player1Controller);
        playerControllers.push_back(player2Controller);

        switch (mode) {
        case GameMode::PVP:
            // PvP : Les deux joueurs sont humains
            player1Controller->switchToHuman();
            player2Controller->switchToHuman();
            break;

        case GameMode::PVE: {
            // PvE : Joueur 1 humain, Joueur 2 IA
            player1Controller->switchToHuman();
            ai::AIInterface* ai = new ai::RandomAI(); // Un exemple simple d'IA
            player2Controller->switchToAI(ai);
            break;
        }

        case GameMode::AIvsAI: {
            // AI vs AI : Les deux joueurs sont des IA
            ai::AIInterface* ai1 = new ai::RandomAI();
            ai::AIInterface* ai2 = new ai::RandomAI(); // On peut utiliser la même IA ou une IA différente
            player1Controller->switchToAI(ai1);
            player2Controller->switchToAI(ai2);
            break;
        }
        }
    }

    PlayerController* ScenarioManager::getPlayerController(state::Player* player) {
        if (player->getPlayerID() == 0) {
            return playerControllers[0];
        }
        else {
            return playerControllers[1];
        }
    }

    GameMode ScenarioManager::getScenarioChoice()
    {
        int choice = -1;

        std::cout<<"Choose a scenario (1: PvP, 2: PvE): "<<std::endl;
        std::cin>>choice;
        while (choice != 1 && choice != 2){
            std::cerr<<"Invalid choice. Please choose a scenario (1: PvP, 2: PvE): "<<std::endl;
            std::cin>>choice;}
        switch(choice)
        {
            case 1:
                return GameMode::PVP;
            case 2:
                return GameMode::PVE;
        }
    }

};

#endif
