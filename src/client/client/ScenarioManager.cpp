#include <iostream>
#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include "client.h"
#include "ai.h"
#include "state.h"


namespace client
{
    ScenarioManager::ScenarioManager(engine::Engine* eng) : engine(eng), mode(GameMode::PVP)
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
            player1Controller = new client::HumanController(engine, 0, nullptr);
            player2Controller = new client::HumanController(engine, 1, nullptr);
            break;

        case GameMode::PVE:
            {

                // PvE : Joueur 1 humain, Joueur 2 IA
                ai::AIInterface *aiModule1;
                std::cout<<"Choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): "<<std::endl;
                int choice;
                std::cin>>choice;
                while(choice != 1 && choice != 2 && choice != 3)
                {
                    std::cerr<<"Invalid choice. Please choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): "<<std::endl;
                    std::cin>>choice;
                }
                switch(choice)
                {
                    case 1:
                        aiModule1 = new ai::RandomAI();
                        break;
                    case 2:
                        aiModule1 = new ai::RandomAI();
                        break;

                    case 3:
                        aiModule1 = new ai::RandomAI();
                        break;
                }
                player1Controller = new HumanController(engine, 0, nullptr);
                player2Controller = new AIController(engine, 1, aiModule1);

                break;
            }

        case GameMode::AIvsAI:
            {
                // AI vs AI : Les deux joueurs sont des IA
                std::cout << "Choose the difficulty of the AI 1 (1: Easy, 2: Medium, 3: Hard): " << std::endl;
                int choice;
                std::cin >> choice;
                while (choice != 1 && choice != 2 && choice != 3)
                {
                    std::cerr << "Invalid choice. Please choose the difficulty of the AI 1 (1: Easy, 2: Medium, 3: Hard): " << std::endl;
                    std::cin >> choice;
                }
                ai::AIInterface* aiModule0;
                switch (choice)
                {
                    case 1:
                        aiModule0 = new ai::RandomAI();
                        break;
                    case 2:
                        aiModule0 = new ai::RandomAI();
                        break;
                    case 3:
                        aiModule0 = new ai::RandomAI();
                        break;
                }
                std::cout << "Choose the difficulty of the AI 2 (1: Easy, 2: Medium, 3: Hard): " << std::endl;
                std::cin >> choice;
                while (choice != 1 && choice != 2 && choice != 3)
                {
                    std::cerr << "Invalid choice. Please choose the difficulty of the AI 2 (1: Easy, 2: Medium, 3: Hard): " << std::endl;
                    std::cin >> choice;
                }
                ai::AIInterface* aiModule1;
                switch (choice)
                {
                    case 1:
                        aiModule1 = new ai::RandomAI();
                        break;
                    case 2:
                        aiModule1 = new ai::RandomAI();
                        break;
                    case 3:
                        aiModule1 = new ai::RandomAI();
                        break;
                }
                player1Controller = new client::AIController(engine, 0, aiModule0);
                player2Controller = new client::AIController(engine, 1, aiModule1);
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
        else
        {
            return playerControllers[1];
        }
    }

    GameMode ScenarioManager::getScenarioChoice()
    {
        int choice = -1;

        std::cout << "Choose a scenario (1: PvP, 2: PvE): " << std::endl;
        std::cin >> choice;
        while (choice != 1 && choice != 2)
        {
            std::cerr << "Invalid choice. Please choose a scenario (1: PvP, 2: PvE): " << std::endl;
            std::cin >> choice;
        }
        switch (choice)
        {
        case 1:
            return GameMode::PVP;
        case 2:
            std::cout<<"Choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): "<<std::endl;
            int choice;
            std::cin>>choice;
            while(choice != 1 && choice != 2 && choice != 3)
            {
                std::cerr<<"Invalid choice. Please choose the difficulty of the AI (1: Easy, 2: Medium, 3: Hard): "<<std::endl;
                std::cin>>choice;
            }
            switch (choice)
            {
            case 1:
                this->aiModule0 = new ai::RandomAI();
                break;
            case 2:
                this->aiModule0 = new ai::RandomAI();
            }


            return GameMode::PVE;
        }
    }

    void ScenarioManager::handlePlacement(state::Game* game)
    {
        for (int i = 0; i < 2; i++)
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
                i--;
                continue;
            }
            engine->handleCmdPlacement(filePath);
        }
    }
};

#endif
