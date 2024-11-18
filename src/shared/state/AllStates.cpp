//
// Created by matthieu on 20/10/24.
//
//
// Created by matthieu on 19/10/24.
//
#include "InitState.h"
#include "PlacementState.h"
#include "PlayerTurnState.h"
#include "WaitingState.h"
#include "WinState.h"
#include "Game.h"
#include <iostream>

#include "Board.h"

namespace state
{
    Board *board=Board::getInstance();

    void InitState::enter(Game* game)
    {
        printf("---InitState ---\n");
        handleInput(game);
    }
    void InitState::handleInput(Game* game)
    {
        update(game);
    }
    void InitState::update(Game* game)
    {
        game->setState(new PlacementState());
    }


    void PlacementState::enter(Game* game)
    {
        printf("--- PlacementState---\n");
        game->switchTurn();
        handleInput(game);

    }
    void PlacementState::handleInput(Game* game)
    {
        auto player = game->getCurrentPlayer();

        std::cout << "Quelle configuration voulez-vous ? \n" << std::endl;
        std::cout << "1 : Offensive, 2 : Defensive, 3 : Balance \n" << std::endl;
        std::string choice;
        std::cin >> choice;
        if (choice == "1") {
            player->chargeConfig("../src/shared/state/config/Offensive.csv");
        }
        else if (choice == "2") {
            player->chargeConfig("../src/shared/state/config/Defensive.csv");
        }
        else if (choice == "3") {
            player->chargeConfig("../src/shared/state/config/Balance.csv");
        }
        else {
            std::cerr << "Incorrect choice entered" << std::endl;
            handleInput(game);
            return;
        }

        game->switchTurn();
        player = game->getCurrentPlayer();

        std::cout << "Quelle configuration voulez-vous ? \n" << std::endl;
        std::cout << "1 : Offensive, 2 : Defensive, 3 : Balance \n" << std::endl;
        std::string choice2;
        std::cin >> choice2;
        if (choice2 == "1") {
            player->chargeConfig("../src/shared/state/config/Offensive.csv");
        }
        else if (choice2 == "2") {
            player->chargeConfig("../src/shared/state/config/Defensive.csv");
        }
        else if (choice2 == "3") {
            player->chargeConfig("../src/shared/state/config/Balance.csv");
        }
        else {
            std::cerr << "Incorrect choice entered" << std::endl;
            game->switchTurn();
            handleInput(game);
            return;
        }

        game->switchTurn();

        update(game);
    }
    void PlacementState::update(Game* game)
    {
        game->setState(new PlayerTurnState());
    }


    void PlayerTurnState::enter(Game* game)
    {
        printf("--- PlayerTurnState ---\n");
        handleInput(game);
        game->switchTurn();
    }

    void PlayerTurnState::handleInput(Game* game) {
        if (game->Purgatory != nullptr) {
            game->getCurrentPlayer()->addCaptured(game->Purgatory);
            game->Purgatory = nullptr;
        }
        int x;
        int y;
        std::cout << "Quelle piece voulez-vous jouer ?" << std::endl;
        std::cin >> x;
        std::cin >> y;
        std::pair<int, int> position;
        position.first=x;
        position.second=y;
        Pieces * pieceToMove = board->getPiece(position);

        if (!game->getCurrentPlayer()->belongTo(pieceToMove)) {
            std::cout << "Ce n'est pas votre piece !" << std::endl;
            handleInput(game);
            return;
        }

        std::cout << "Quelle est votre destination ?" << std::endl;
        int newx;
        int newy;
        std::cin >> newx;
        std::cin >> newy;
        std::pair<int, int> destination;
        destination.first=newx;
        destination.second=newy;
        if (pieceToMove->CheckBoard(destination)) {
            if (pieceToMove->CheckRange(destination)) {
                if (pieceToMove->CheckCase(destination) == "Empty" ||
                    pieceToMove->CheckCase(destination) == "Enemy") {
                    update(game);
                    return;
                }
                handleInput(game);
            }
        }
    }

    void PlayerTurnState::update(Game* game)
    {
        Player * player=game->getCurrentPlayer();
        std::vector<Pieces*> capturedPieces=player->getCaptured();
        if(capturedPieces[0]->getValue()==0) {
            game->setState(new WinState());
        }
        else {
            game->setState(new PlayerTurnState());
        }
    }

    void WinState::enter(Game* game)
    {
        printf("--- WinState ---\n");
        handleInput(game);
        //game->endGame();
    }
    void WinState::handleInput(Game* game)
    {
        update(game);
    }
    void WinState::update(Game* game)
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
            update(game);
        }
        else
        {
            std::cout << "\nThank you for playing! See you next time." << std::endl;
            exit(0);
        }
    }
    void WaitingState::update(Game* game)
    {
        game->setState(new InitState());
    }


    State::~State() {}

}

