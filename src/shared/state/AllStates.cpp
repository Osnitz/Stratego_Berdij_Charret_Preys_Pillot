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
#include "IATurnState.h"
#include <iostream>
#include <random>
#include "Board.h"

namespace state
{
    Board *board=Board::getInstance();

    void InitState::enter(Game* game)
    {
        printf("---InitState ---\n");
        printf("Welcome to the game!\n");
        printf("Do you want to play against the AI or another Player? (1/2): \n");
        std::string answer;
        std::cin >> answer;
        if (answer == "1")
        {
            game->setAI(true);
        }
        else
        {
            game->setAI(false);
        }
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
        update(game);
    }
    void PlacementState::update(Game* game)
    {
        if (game->getCurrentPlayer() == game->getPlayer2() && game->againstIA) {
            game->setState(new IATurnState());
        } else {
            game->setState(new PlayerTurnState());
        }
    }


    void PlayerTurnState::enter(Game* game)
    {
        printf("--- PlayerTurnState ---\n");
        std::cout << "C'est le tour du joueur: " << (game->getCurrentPlayer()== game->getPlayer1() ? "Player 1" : "Player 2") << std::endl;
        handleInput(game);
    }
    void PlayerTurnState::handleInput(Game* game) {
        if (game->Purgatory != nullptr) {
            game->getCurrentPlayer()->addCaptured(game->Purgatory);
            game->Purgatory = nullptr;
        }
        if (game->Graveyard != nullptr) {
            game->getCurrentPlayer()->removePiece(game->Graveyard);
            game->Graveyard = nullptr;
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

        while (true) {
            if (!game->getCurrentPlayer()->belongTo(pieceToMove)) {
                std::cout << "Ce n'est pas votre piece !" << std::endl;
                continue;
            }
            break;
        }

        auto possiblePositions = pieceToMove->canMove(pieceToMove);
        if (possiblePositions.empty()) {
            std::cout << "Aucun mouvement possible pour cette piece." << std::endl;
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

        for(auto possible_position : possiblePositions)
        {
            if(possible_position==destination)
            {
                pieceToMove->setPosition(destination);
                update(game);
                return;
            }
        }
        std::cerr<<"Destination non valide"<<std::endl;
        handleInput(game);
    }
    void PlayerTurnState::update(Game* game)
    {
        Player * player=game->getCurrentPlayer();
        std::vector<Pieces*>  capturedPieces=player->getCaptured();
        if(capturedPieces[0]->getValue()==0) {
            game->setState(new WinState());
        }
        else {
            game->switchTurn();
            if (game->getCurrentPlayer() == game->getPlayer2() && game->againstIA) {
                game->setState(new IATurnState());
            } else {
                game->setState(new PlayerTurnState());
            }
        }
    }


    void IATurnState::enter(Game* game)
    {
        printf("--- IATurnState ---\n");
        handleInput(game);
    }

    void IATurnState::handleInput(Game* game)
    {
        Player* aiPlayer = game->getCurrentPlayer();
        std::vector<Pieces*> aiPieces = aiPlayer->getMyPieces();

        if (aiPieces.empty()) {
            std::cout << "No pieces available for AI to move." << std::endl;
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, aiPieces.size() - 1);
        Pieces* pieceToMove = aiPieces[distr(gen)];
        auto possiblePositions = pieceToMove->canMove(pieceToMove);
        while(possiblePositions.empty())
        {
            pieceToMove = aiPieces[distr(gen)];
            possiblePositions = pieceToMove->canMove(pieceToMove);
        }
        std::uniform_int_distribution<> distr2(0, possiblePositions.size() - 1);
        auto destination = possiblePositions[distr2(gen)];
        pieceToMove->setPosition(destination);
        update(game);
    }

    void IATurnState::update(Game* game)
    {
        game->switchTurn();
        game->setState(new PlayerTurnState());
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

