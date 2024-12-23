//
// Created by matthieu on 23/12/24.
//

#define BOOST_TEST_MODULE BoardTest
#include <boost/test/included/unit_test.hpp>
#include "client.h"
using namespace state;
using namespace client;

BOOST_AUTO_TEST_SUITE(HumanControllerTestSuite)
    BOOST_AUTO_TEST_CASE(TestConstructor)
    {
        auto mockEngine = new engine::Engine(new Game());

        auto humanController = new client::HumanController(mockEngine, nullptr);

        BOOST_CHECK(humanController->getEngine() == mockEngine);

        BOOST_CHECK(humanController->getAiModule() == nullptr);

        delete mockEngine;
    }

    BOOST_AUTO_TEST_CASE(TestGetPlayerInput)
    {
        auto mockEngine = new engine::Engine(new Game());

        auto humanController = new client::HumanController(mockEngine, nullptr);

        std::istringstream input("2 3\n2 4\n");
        std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
        std::vector<int> coords = humanController->getPlayerInput();

        BOOST_CHECK(coords[0] == 2);
        BOOST_CHECK(coords[1] == 3);
        BOOST_CHECK(coords[2] == 2);
        BOOST_CHECK(coords[3] == 4);

        delete mockEngine;
    }

    BOOST_AUTO_TEST_CASE(TestExecuteCmd)
    {
        auto game = new Game();
        auto mockEngine = new engine::Engine(game);
        auto mypiece = new Pieces(2, Scout, 0, 0, game->getPlayer1());
        game->currentPlayer = game->getPlayer1();
        game->addPiece(mypiece, game->currentPlayer);
        game->SetPieceOnBoard(mypiece, 0, 0);


        auto humanController = new client::HumanController(mockEngine, nullptr);
        //Case 1 : Valid move
        std::pair<int, int> from = std::make_pair(0, 0);
        std::pair<int, int> to = std::make_pair(0, 2);

        BOOST_CHECK(humanController->executeCmd(from, to, game->currentPlayer) == true);

        //Case 2 : Invalid move
        from = std::make_pair(0, 2);
        to = std::make_pair(1, 1);
        BOOST_CHECK(humanController->executeCmd(from, to, game->currentPlayer) == false);

        delete game;
        delete mockEngine;
        delete mypiece;
    }

    BOOST_AUTO_TEST_CASE(TestHandlePlacement)
    // Issue with path construction, either it works running client, either it works running tests
    {
        auto game = new Game();
        auto mockEngine = new engine::Engine(game);

        auto humanController = new client::HumanController(mockEngine, nullptr);
        // Case 1 : Offensive
        game->currentPlayer = game->getPlayer1();
        std::istringstream input("1\n");
        std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
        humanController->handlePlacement(mockEngine->getGame());
        game->currentPlayer = game->getPlayer1(); // after placement, currentPlayer is switched
        BOOST_CHECK(mockEngine->getGame()->getPlayer1()->getMyPieces().size() == 40);
        game->getPlayer1()->getMyPieces().clear();

        // Case 2 : Defensive
        game->currentPlayer = game->getPlayer1();
        std::istringstream input2("2\n");
        std::cin.rdbuf(input2.rdbuf()); // Redirect std::cin to use input stream
        humanController->handlePlacement(mockEngine->getGame());
        game->currentPlayer = game->getPlayer1(); // after placement, currentPlayer is switched

        BOOST_CHECK(mockEngine->getGame()->getPlayer1()->getMyPieces().size() == 40);
        game->getPlayer1()->getMyPieces().clear();

        // Case 3 : Balance
        game->currentPlayer = game->getPlayer1();
        std::istringstream input3("0\n3\n"); //0 is invalid, 3 is valid
        std::cin.rdbuf(input3.rdbuf()); // Redirect std::cin to use input stream
        humanController->handlePlacement(mockEngine->getGame());
        game->currentPlayer = game->getPlayer1(); // after placement, currentPlayer is switched

        BOOST_CHECK(mockEngine->getGame()->getPlayer1()->getMyPieces().size() == 40);

        delete mockEngine;
        delete game;
    }

    BOOST_AUTO_TEST_CASE(TestDestructor)
    {
        auto mockEngine = new engine::Engine(new Game());

        auto humanController = new client::HumanController(mockEngine, nullptr);

        delete humanController;
    }


BOOST_AUTO_TEST_SUITE_END()
