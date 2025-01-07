//
// Created by plt on 06/01/25.
//

#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include "engine.h"
#include "ai.h"

using namespace state;
using namespace engine;
using namespace ai;

BOOST_AUTO_TEST_SUITE(GameTestSuite)

BOOST_AUTO_TEST_CASE(testClearCache) {
    HeuristicAI* ai=new HeuristicAI();
    BOOST_CHECK(ai->possibleMovesCache.empty());
}

BOOST_AUTO_TEST_CASE(testGetPossibleMoves) {
    HeuristicAI* ai=new HeuristicAI();
    Game* game=new Game();
    Player* player=game->getCurrentPlayer();
    Pieces piece(1, Scout, 0, 0,player);
    game->setPieceOnBoard(&piece,0,0);
    BOOST_CHECK(!ai->getPossibleMoves(&piece,game).empty());
}

BOOST_AUTO_TEST_SUITE_END()