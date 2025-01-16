#include <iostream>
#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "client.h"
#include "ai.h"
#include "state.h"


namespace client
{
    ScenarioManager::ScenarioManager(engine::Engine* eng) : engine(eng), mode(GameMode::PVP), aiModule0(nullptr),
    aiModule1(nullptr)
    {
    }

    // Configure le mode de jeu
    void ScenarioManager::setMode(GameMode m)
    {
        mode = m;
    }

    // Initialise les PlayerControllers en fonction du mode de jeu
    void ScenarioManager::initializeControllers()
    {
        PlayerController* player1Controller;
        PlayerController* player2Controller;
        switch (mode)
        {
        case GameMode::PVP:
            // PvP : Les deux joueurs sont humains
            player1Controller = new client::HumanController(engine, nullptr);
            player2Controller = new client::HumanController(engine, nullptr);
            break;

        case GameMode::PVE:
            {
                // PvE : Joueur 1 humain, Joueur 2 IA

                player1Controller = new HumanController(engine, nullptr);
                player2Controller = new AIController(engine, aiModule0);

                break;
            }

        case GameMode::AIvsAI:
            {
                // AI vs AI : Les deux joueurs sont des IA
                player1Controller = new client::AIController(engine, aiModule0);
                player2Controller = new client::AIController(engine, aiModule1);
                break;
            }
        }
        playerControllers.push_back(player1Controller);
        playerControllers.push_back(player2Controller);
    }

    PlayerController* ScenarioManager::getPlayerController(state::Player* player)
    {
        if (player->getPlayerID() == 0)
        {
            return playerControllers[0];
        }
            return playerControllers[1];
    }

    GameMode ScenarioManager::getScenarioChoice()
    {
        int choice = -1;

        std::cout << "Choose a scenario (1: PvP, 2: PvE, 3: AIvsAI): " << std::endl;
        std::cin >> choice;
        while (choice != 1 && choice != 2 && choice != 3)
        {
            std::cerr << "Invalid choice. Please choose a scenario (1: PvP, 2: PvE, 3: AIvsAI): " << std::endl;
            std::cin >> choice;
        }
        switch (choice)
        {
        case 1:
            return GameMode::PVP;
        case 2:
            std::cout << "Choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): " << std::endl;
            std::cin >> choice;
            while (choice != 1 && choice != 2 && choice != 3)
            {
                std::cerr << "Invalid choice. Please choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): " <<
                    std::endl;
                std::cin >> choice;
            }
            switch (choice)
            {
            case 1:
                this->aiModule0 = new ai::RandomAI();
                break;
            case 2:
                this->aiModule0 = new ai::HeuristicAI();
                break;
            case 3:
                this->aiModule0 = new ai::AdvancedAI();
                break;
            }
            return GameMode::PVE;
        case 3:
            std::cout << "Choose the difficulty of the AI 0 (1: Easy, 2: Medium, 3: Hard): " << std::endl;
            int choiceAI1;
            std::cin >> choiceAI1;
            while (choiceAI1 != 1 && choiceAI1 != 2 && choiceAI1 != 3)
            {
                std::cerr << "Invalid choice. Please choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): " <<
                    std::endl;
                std::cin >> choiceAI1;
            }
            switch (choiceAI1)
            {
            case 1:
                this->aiModule0 = new ai::RandomAI();
                break;
            case 2:
                this->aiModule0 = new ai::HeuristicAI();
                break;
            case 3:
                this->aiModule0 = new ai::RandomAI();
                break;
            }
            std::cout << "Choose the difficulty of the AI 1 (1: Easy, 2: Medium, 3: Hard): " << std::endl;
            int choiceAI2;
            std::cin >> choiceAI2;
            while (choiceAI2 != 1 && choiceAI2 != 2 && choiceAI2 != 3)
            {
                std::cerr << "Invalid choice. Please choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): " <<
                    std::endl;
                std::cin >> choiceAI2;
            }
            switch (choiceAI2)
            {
            case 1:
                aiModule1 = new ai::RandomAI();
                break;
            case 2:
                aiModule1 = new ai::HeuristicAI();
                break;
            case 3:
                aiModule1 = new ai::RandomAI();
                break;
            }
            return GameMode::AIvsAI;
        }
    }

    ScenarioManager::~ScenarioManager()
    {

    }

    engine::Engine* ScenarioManager::getEngine()
    {
        return engine;
    }

    GameMode ScenarioManager::getMode()
    {
        return mode;
    }

    std::vector<PlayerController*>* ScenarioManager::getPlayerControllersVector()
    {
        return &playerControllers;
    }

    ai::AIInterface* ScenarioManager::getAiModule0()
    {
        return aiModule0;
    }

    ai::AIInterface* ScenarioManager::getAiModule1()
    {
        return aiModule1;
    }

}

#endif
