//
// Created by matthieu on 19/10/24.
//
#include "BattleState.h"
#include "InitState.h"
#include "PlacementState.h"
#include "PlayerTurnState.h"
#include "WaitingState.h"
#include "WinState.h"
#include "Game.h"
#include <iostream>
namespace state
{
    void InitState::enter(Game* game)
    {
        printf("---InitState---\n");
        handleInput(game);
    }
    void InitState::handleInput(Game* game)
    {
        game->setState(new PlacementState());
    }


    void PlacementState::enter(Game* game)
    {
        printf("--- PlacementState---\n");
        handleInput(game);
        //game->switchTurn();
    }
    void PlacementState::handleInput(Game* game)
    {
        game->setState(new PlayerTurnState());
    }


    void PlayerTurnState::enter(Game* game)
    {
        printf("--- PlayerTurnState ---\n");
        handleInput(game);
        //game->switchTurn();
    }
    void PlayerTurnState::handleInput(Game* game)
    {
        game->setState(new BattleState());
    }


    void BattleState::enter(Game* game)
    {
        printf("--- BattleState ---\n");
        handleInput(game);
        //game->checkVictory();
    }
    void BattleState::handleInput(Game* game)
    {
        game->setState(new WinState());
    }


    void WinState::enter(Game* game)
    {
        printf("--- WinState ---\n");
        handleInput(game);
        //game->endGame();
    }
    void WinState::handleInput(Game* game)
    {
        game->setState(new WaitingState());
    }

    void WaitingState::enter(Game* game)
    {
        printf("--- WaitingState ---\n");
        handleInput(game);
    }
    void WaitingState::handleInput(Game* game)
    {
        printf("Do you want to play again? (y/n): \n");
        char answer;
        std::cin >> answer;
        if (answer == 'y')
        {
            game->setState(new InitState());
        }
        else
        {
            std::cout << "\nThank you for playing! See you next time." << std::endl;
            exit(0);
        }
    }


    State::~State() {}

}

