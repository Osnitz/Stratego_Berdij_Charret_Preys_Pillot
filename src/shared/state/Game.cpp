//
// Created by matthieu on 14/10/24.
//
#include "Game.h"

#include <stdexcept>

#include "Board.h"
#include "Player.h"
#include "InitState.h"
#include "random"
namespace state {
     Game* Game::instance=nullptr;

    Game::Game() {
        board = Board::getInstance();
        Player1 = new Player();
        Player2 = new Player();
        currentState = new InitState();
        currentPlayer = 0;
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
        if(currentPlayer == 0) {
            std::random_device rd;  // Obtain a random number from hardware
            std::mt19937 gen(rd()); // Seed the generator
            std::uniform_int_distribution<> distr(1, 2); // Define the range
            currentPlayer = distr(gen); // Generate the random number
        }
        if (currentPlayer == 1) {
            currentPlayer = 2;
        } else {
            currentPlayer = 1;
        }
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

    Player * Game::getCurrentPlayer() {
        if(currentPlayer == 0) {
            throw std::invalid_argument("No current player");
        }
        if (currentPlayer == 1) {
            return Player1;
        }
        return Player2;
    }
}
