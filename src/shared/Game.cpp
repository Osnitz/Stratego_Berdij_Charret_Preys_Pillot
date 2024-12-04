//
// Created by matthieu on 14/10/24.
//
#include <iostream>
#include <stdexcept>

#include "state.h"
#include "engine.h"
#include "random"
using namespace state;
using namespace engine;
using namespace std;

Game* Game::instance=nullptr;

Game::Game() {
    board = Board::getInstance();
    Player1 = new Player();
    Player2 = new Player();
    currentState = new InitState();
    currentPlayer = nullptr;
    againstIA = false;
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
        if(currentPlayer == nullptr) {
            std::random_device rd;  // Obtain a random number from hardware
            std::mt19937 gen(rd()); // Seed the generator
            std::uniform_int_distribution<> distr(1, 2); // Define the range
            int randomPlayer = distr(gen); // Generate the random number
            if (randomPlayer == 1) {
                currentPlayer = Player1;
            } else {
                currentPlayer = Player2;
            }
        } else {
            if (currentPlayer == Player1) {
                currentPlayer = Player2;
            } else {
                currentPlayer = Player1;
            }
        }
    }

    void Game::setState(engine::State * state) {
        currentState = state;
        currentState->enter(this);
    }

    Player* Game::getCurrentPlayer() {
        if (currentPlayer == nullptr) {
            throw std::invalid_argument("No current player");
        }
        return currentPlayer;
    }

    void Game::setAI(bool AIvalue) {
        againstIA = AIvalue;
    }

    Player* Game::getPlayer1() {
        return Player1;
    }
    Player* Game::getPlayer2() {
        return Player2;
    }

void Game::removePiece(Pieces* piece) {
    vector<Pieces*> myPieces=currentPlayer->getMyPieces();
        if (myPieces.empty()) {
            cerr<<"No piece left"<<endl;
            return;
        }
        for (size_t i = 0; i < myPieces.size(); i++) {
            if (myPieces[i] == piece) {
                myPieces.erase(myPieces.begin() + i);
                return;
            }
        }
        cerr<<"Can't remove this piece : it doesn't exist!"<<endl;
    }