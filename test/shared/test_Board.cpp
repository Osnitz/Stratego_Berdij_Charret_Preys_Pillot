#define BOOST_TEST_MODULE BoardTest
#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include <iostream>

using namespace state;

BOOST_AUTO_TEST_SUITE(BoardTestSuite)

BOOST_AUTO_TEST_CASE(testBoard) {
    Board board;
    auto grid=board.getGrid();
    BOOST_CHECK(grid->size()==10);
    BOOST_CHECK(grid[0].size()==10);
}
BOOST_AUTO_TEST_CASE(testgetPiece) {
    Game* game=new Game();
    auto board=game->getBoard();
    Player* player=game->getPlayer1();
    Pieces piece(1, Scout, 0, 0,player);
    game->setPieceOnBoard(&piece,0,0);

    BOOST_CHECK(board->getPiece({0,0})==&piece);
    BOOST_CHECK(board->getPiece({1,0})==nullptr);
    BOOST_CHECK(board->getPiece({10,10})==nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
