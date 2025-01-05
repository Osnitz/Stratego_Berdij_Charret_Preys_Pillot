//
// Created by matthieu on 23/12/24.
//
#include <ai/RandomAI.h>
#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include "engine.h"

using namespace state;
BOOST_AUTO_TEST_SUITE(RandomAITestSuite)

BOOST_AUTO_TEST_CASE(TestCalculateMove)
{
    // Initialize the game and ai
    auto game = new Game();
    auto engine = new engine::Engine(game);
    auto board = game->getBoard();

    auto ai = new ai::RandomAI();
    auto player = game->getPlayer1();
    game->setCurrentPlayer(player);

    // Case 1: No pieces available
    auto move = ai->calculateMove(game);
    BOOST_CHECK_MESSAGE(move.empty(), "Should return an empty list");

    // Case 2: No valid moves available
    auto flag = new Pieces(0, PieceType::Flag, 0, 0, game->getPlayer1());
    auto scout = new Pieces(2, PieceType::Scout, 0, 9, game->getPlayer1());
    auto bomb1 = new Pieces(11, PieceType::Bomb, 0, 8, game->getPlayer1());
    auto bomb2 = new Pieces(11, PieceType::Bomb, 1, 9, game->getPlayer1());
    game->addPiece(flag, game->getPlayer1());
    game->setPieceOnBoard(flag, 0, 0);
    game->addPiece(scout, game->getPlayer1());
    game->setPieceOnBoard(scout, 0, 9);
    game->addPiece(bomb1, game->getPlayer1());
    game->setPieceOnBoard(bomb1, 0, 8);
    game->addPiece(bomb2, game->getPlayer1());
    game->setPieceOnBoard(bomb2, 1, 9);
    game->displayBoard(*player);
    auto move2 = ai->calculateMove(game);
    BOOST_CHECK_MESSAGE(move2.empty(), "Should return an empty list");

    // Case 3: Valid move is possible
    auto marshal = new Pieces(10, PieceType::Marshal, 4, 0, game->getPlayer1());
    game->addPiece(marshal, game->getPlayer1());
    game->setPieceOnBoard(marshal, 4, 0);
    auto move3 = ai->calculateMove(game);
    BOOST_CHECK_MESSAGE(move3.size()==4, "Should return a non-empty list of 4 coordinates");
    std::pair<int,int> from = std::make_pair(move3[0], move3[1]);
    std::pair<int,int> to = std::make_pair(move3[2], move3[3]);
    auto pieceToMove = board->getPiece(from);
    BOOST_CHECK_MESSAGE(engine->isValidMove(pieceToMove,to), "Should return a valid move");

    delete game;
    delete engine;
    delete ai;
}
BOOST_AUTO_TEST_SUITE_END()