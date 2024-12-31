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

#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include "engine.h"

using namespace state;
using namespace engine;

BOOST_AUTO_TEST_SUITE(GameTestSuite)

// Test de l'initialisation des membres
BOOST_AUTO_TEST_CASE(testConstructor) {
    Game* game=new Game() ;
    BOOST_CHECK(game->getPlayer1() != nullptr);
    BOOST_CHECK(game->getPlayer2() != nullptr);
    BOOST_CHECK(game->getCurrentPlayer() != nullptr);
    BOOST_CHECK(game->getCurrentPlayer() == game->getPlayer1()||game->getCurrentPlayer()== game->getPlayer2());
    BOOST_CHECK(game->getBoard() != nullptr);
}

// Test de la méthode switchTurn
BOOST_AUTO_TEST_CASE(testSwitchTurn) {
    Game* game=new Game() ;
    Player* firstPlayer = game->getCurrentPlayer();

    // Test du changement de joueur
    game->switchTurn();
    Player* secondPlayer = game->getCurrentPlayer();
    BOOST_CHECK(secondPlayer != nullptr);
    BOOST_CHECK(firstPlayer != secondPlayer);

    // Test du retour au premier joueur
    game->switchTurn();
    BOOST_CHECK(firstPlayer == game->getCurrentPlayer());
}

BOOST_AUTO_TEST_CASE(testAddCaptured) {
    Game* game=new Game() ;
    auto player=game->getCurrentPlayer();
    Pieces piece(1, Scout, 0, 0,player);
    Pieces miner(2, Miner, 0, 0,player);
    Pieces flag(0, Flag, 0, 0,player);
    game->addCaptured(&piece,player);
    auto& capturedPieces = player->getCaptured();
    BOOST_CHECK(capturedPieces[0] == &piece);
    game->addCaptured(&miner,player);
    game->addCaptured(&flag,player);
    BOOST_CHECK(capturedPieces[0] == &flag);
    BOOST_CHECK(capturedPieces[1] == &piece);
    BOOST_CHECK(capturedPieces[2] == &miner);
}

BOOST_AUTO_TEST_CASE(testRemovePiece) {
    Game* game=new Game();
    auto player=game->getCurrentPlayer();
    auto& mypiece=player->getMyPieces();
    Pieces piece(1, Scout, 0, 0,player);
    Pieces miner(2, Miner, 0, 0,player);
    game->addPiece(&piece,player);
    BOOST_TEST_MESSAGE("piece doesn't exist");
    game->removePiece(&miner,player);
    game->removePiece(&piece,player);
    BOOST_CHECK(mypiece.empty());
    BOOST_TEST_MESSAGE("my piece is empty:");
    game->removePiece(&miner,player);

}

BOOST_AUTO_TEST_CASE(testGetOpponent) {
    Game* game=new Game() ;
    Player* firstPlayer = game->getCurrentPlayer();
    BOOST_CHECK(firstPlayer!=game->getOpponent());
    game->switchTurn();
    BOOST_CHECK(firstPlayer==game->getOpponent());
}

BOOST_AUTO_TEST_CASE(testBelongTo) {
    Game* game=new Game() ;
    auto player=game->getCurrentPlayer();
    Pieces piece(1, Scout, 0, 0,player);
    Pieces miner(2, Miner, 0, 0,player);
    game->addPiece(&piece,player);
    BOOST_CHECK(game->belongTo(&piece));
    BOOST_CHECK(!game->belongTo(&miner));
}

BOOST_AUTO_TEST_CASE(testAddPiece) {
    Game* game=new Game();
    auto player=game->getCurrentPlayer();
    auto& mypiece=player->getMyPieces();
    Pieces piece(1, Scout, 0, 0,player);
    game->addPiece(&piece,player);
    BOOST_CHECK(mypiece[0]==&piece);
}
BOOST_AUTO_TEST_SUITE_END()
