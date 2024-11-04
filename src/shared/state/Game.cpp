//
// Created by matthieu on 14/10/24.
//
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "InitState.h"

namespace state {

    Game::Game() {
        board = Board::getInstance();
        Player1 = new Player();
        Player2 = new Player();
        currentState = new InitState();
    }

    Game::~Game() {
        delete board;
        delete Player1;
        delete Player2;
        delete currentState;
    }

    Game* Game::getInstance()
    {
        if (instance == nullptr)
        {
            instance = new Game();
        }
        return instance;
    }

    void Game::startGame() {
        currentState-> enter(this);
    }

    void Game::switchTurn() {
        // TODO implement here
    }

    void Game::checkVictory() {
        // TODO implement here
    }

    void Game::endGame() {
        // TODO implement here
    }

    void Game::setState(State * state) {
        currentState = state;
        currentState->enter(this);
    }
}
