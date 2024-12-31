//
// Created by matthieu on 23/12/24.
//
#define BOOST_TEST_MODULE BoardTest
#include <ai/RandomAI.h>
#include <boost/test/included/unit_test.hpp>
#include "client.h"
using namespace state;
using namespace client;

BOOST_AUTO_TEST_SUITE(AIControllerTestSuite)
    BOOST_AUTO_TEST_CASE(TestConstructor)
    {
        auto mockEngine = new engine::Engine(new Game());

        auto aiController = new client::AIController(mockEngine, nullptr);

        BOOST_CHECK(aiController->getEngine() == mockEngine);

        BOOST_CHECK(aiController->getAiModule() == nullptr);

        delete mockEngine;
    }

    BOOST_AUTO_TEST_CASE(TestGetPlayerInput)
    {
        auto game = new Game();
        auto mockEngine = new engine::Engine(game);

        auto aiController = new client::AIController(mockEngine, new ai::RandomAI());
        auto mypiece = new Pieces(2, Scout, 0, 0, game->getPlayer1());
        game->addPiece(mypiece, game->getCurrentPlayer());
        game->setPieceOnBoard(mypiece, 0, 0);
        game->displayBoard(*game->getCurrentPlayer());
        std::vector<int> coords = aiController->getPlayerInput();

        BOOST_CHECK(coords.size() == 4);

        delete mockEngine;
        delete game;
        delete mypiece;
    }

    BOOST_AUTO_TEST_CASE(TestExecuteCmd)
    {
        auto game = new Game();
        auto mockEngine = new engine::Engine(game);
        auto aiController = new client::AIController(mockEngine, new ai::RandomAI());
        auto mypiece = new Pieces(2, Scout, 0, 0, game->getPlayer1());
        game->setCurrentPlayer(game->getPlayer1());
        game->addPiece(mypiece, game->getCurrentPlayer());
        game->setPieceOnBoard(mypiece, 0, 0);

        //Case 1 : Valid move
        std::pair<int, int> from = std::make_pair(0, 0);
        std::pair<int, int> to = std::make_pair(0, 2);
        BOOST_CHECK(aiController->executeCmd(from, to, game->getCurrentPlayer())==true);

        //Case 2 : Invalid move
        game->setCurrentPlayer(game->getPlayer1());
        from = std::make_pair(0, 2);
        to = std::make_pair(1, 1);
        BOOST_CHECK(aiController->executeCmd(from, to, game->getCurrentPlayer())==false);
    }

    BOOST_AUTO_TEST_CASE(TestHandlePlacement)
    {
        auto game = new Game();
        auto mockEngine = new engine::Engine(game);
        auto aiController = new client::AIController(mockEngine, new ai::RandomAI());
        aiController->handlePlacement(game);
        game->displayBoard(*game->getCurrentPlayer());
        delete game;
        delete mockEngine;
    }

    BOOST_AUTO_TEST_CASE(TestDestructor)
    {
        auto game = new Game();
        auto mockEngine = new engine::Engine(game);
        auto aiController = new client::AIController(mockEngine, new ai::RandomAI());
        delete aiController;
    }

BOOST_AUTO_TEST_SUITE_END()
