/*//
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
}

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
}*/

#define BOOST_TEST_MODULE GameTest
#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include "engine.h"

using namespace state;
using namespace engine;

BOOST_AUTO_TEST_SUITE(GameTestSuite)

// Test du Singleton
BOOST_AUTO_TEST_CASE(testSingletonInstance) {
    Game* game1 = Game::getInstance();
    Game* game2 = Game::getInstance();
    BOOST_CHECK(game1 == game2);
}

// Test de l'initialisation des membres
BOOST_AUTO_TEST_CASE(testInitialization) {
    Game* game = Game::getInstance();
    BOOST_CHECK(game->getPlayer1() != nullptr);
    BOOST_CHECK(game->getPlayer2() != nullptr);
    BOOST_CHECK(game->getCurrentPlayer() != nullptr);
    BOOST_CHECK(game->againstIA == false); // Pas d'IA au début
}


// Test de la méthode switchTurn
BOOST_AUTO_TEST_CASE(testSwitchTurn) {
    Game* game = Game::getInstance();

    // Test du premier tirage au sort
    BOOST_CHECK_NO_THROW(game->switchTurn());
    Player* firstPlayer = game->getCurrentPlayer();
    BOOST_CHECK(firstPlayer != nullptr);

    // Test du changement de joueur
    game->switchTurn();
    Player* secondPlayer = game->getCurrentPlayer();
    BOOST_CHECK(secondPlayer != nullptr);
    BOOST_CHECK(firstPlayer != secondPlayer);
}

// Test de la gestion de l'IA
BOOST_AUTO_TEST_CASE(testSetAI) {
    Game* game = Game::getInstance();

    game->setAI(true);
    BOOST_CHECK(game->againstIA);

    game->setAI(false);
    BOOST_CHECK(!game->againstIA);
}

// Test de la méthode getCurrentPlayer avec exception
BOOST_AUTO_TEST_CASE(testGetCurrentPlayerException) {
    Game* game = Game::getInstance();

    // Réinitialiser pour s'assurer qu'aucun joueur n'est sélectionné
    game->currentPlayer = nullptr;
    BOOST_CHECK_THROW(game->getCurrentPlayer(), std::invalid_argument);
}

// Test de changement d'état
/*BOOST_AUTO_TEST_CASE(testStateTransition) {
    Game* game = Game::getInstance();
    State* newState = new InitState();

    BOOST_CHECK_NO_THROW(game->setState(newState)); // Transition vers un nouvel état
    delete newState; // Nettoyage
}*/

BOOST_AUTO_TEST_SUITE_END()
