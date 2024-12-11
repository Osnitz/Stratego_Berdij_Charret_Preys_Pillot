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

    Player* Game::getOpponent() const {
        if (currentPlayer == Player1) {
            return Player2;
        } else if (currentPlayer == Player2) {
            return Player1;
        }
        return nullptr;
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

void Game::removePiece(const Pieces* piece, Player* player) {
        std::vector<Pieces*>& myPieces = player->getMyPieces();

        if (myPieces.empty()) {
            cerr << "No piece left" << endl;
            return;
        }

        for (auto it = myPieces.begin(); it != myPieces.end(); ++it) {
            if (*it == piece) {
                myPieces.erase(it);
                return;
            }
        }

        cerr << "Can't remove this piece: it doesn't exist!" << endl;
    }

void Game::addCaptured(Pieces* piece, Player* player) {
        Board::getInstance()->removeFromBoard(piece);
        const int value = piece->getValue();

        for (auto it = player->capturedPieces.begin(); it != player->capturedPieces.end(); ++it) {
            int myvalue = (*it)->getValue();
            if (value <= myvalue) {
                player->capturedPieces.insert(it, piece);
                return;
            }
        }
        player->capturedPieces.push_back(piece);
}
