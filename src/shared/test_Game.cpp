//
// Created by matthieu on 06/11/24.
//
#include <iostream>
#include <stdexcept>
#include "state.h"
#include "engine.h"

using namespace state;

void testSingletonInstance() {
    Game* game1 = Game::getInstance();
    Game* game2 = Game::getInstance();
    if (game1 != game2) {
        std::cerr << "SingletonInstance test failed!" << std::endl;
    } else {
        std::cout << "SingletonInstance test passed!" << std::endl;
    }
}

/*void testStartGame() {
    Game* game = Game::getInstance();
    try {
        game->startGame();
        std::cout << "StartGame test passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "StartGame test failed: " << e.what() << std::endl;
    }
}*/

void testSwitchTurn() {
    Game* game = Game::getInstance();
    game->switchTurn();
    Player* currentPlayer = game->getCurrentPlayer();
    if (currentPlayer == nullptr) {
        std::cerr << "SwitchTurn test failed!" << std::endl;
    } else {
        game->switchTurn();
        Player* nextPlayer = game->getCurrentPlayer();
        if (nextPlayer == nullptr || nextPlayer == currentPlayer) {
            std::cerr << "SwitchTurn test failed!" << std::endl;
        } else {
            std::cout << "SwitchTurn test passed!" << std::endl;
        }
    }
}

void testGetCurrentPlayer() {
    Game* game = Game::getInstance();
    try {
        Player* player = game->getCurrentPlayer();
        if (player != nullptr) {
            std::cout << "GetCurrentPlayer test passed!" << std::endl;
        } else {
            std::cerr << "GetCurrentPlayer test failed!" << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "GetCurrentPlayer test failed: " << e.what() << std::endl;
    }
}

int main() {
    testSingletonInstance();
    //testStartGame();
    testSwitchTurn();
    testGetCurrentPlayer();
    return 0;
}