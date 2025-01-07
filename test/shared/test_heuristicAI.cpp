//
// Created by plt on 07/01/25.
//

#include <ai/HeuristicAI.h>
#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include "engine.h"

using namespace state;
using namespace engine;
using namespace ai;

BOOST_AUTO_TEST_SUITE(HeuristicAITestSuite)

BOOST_AUTO_TEST_CASE(testGetPossibleMovesClearCache) {
    HeuristicAI* ai=new HeuristicAI();
    Game* game=new Game();
    Player* player=game->getCurrentPlayer();
    Pieces piece(1, Scout, 0, 0,player);
    game->setPieceOnBoard(&piece,0,0);
    BOOST_CHECK(!ai->getPossibleMoves(&piece,game).empty());
    ai->clearCache();
    BOOST_CHECK(ai->possibleMovesCache.empty());
}

BOOST_AUTO_TEST_CASE(testHeuristicCalculator) {
    HeuristicAI* ai=new HeuristicAI();
    Game* game=new Game();
    auto player=game->getCurrentPlayer();
    auto opponent=game->getOpponent();
    Pieces piece(1, Scout, 0, 0,player);

    //case scout
    std::pair<int,int> newpos={0,1};
    BOOST_CHECK(ai->heuristicCalculator(&piece,newpos,game)==60);

    //case dist=0
    Pieces miner(2, Miner, 0, 0,player);
    Pieces scoutE(1, Scout, 5, 4,opponent);
    game->addKnown(&scoutE,player);
    newpos={5,6};
    BOOST_CHECK(ai->heuristicCalculator(&miner,newpos,game)==110);

    //case mypiece>ennemy
    Pieces sergeant(3, Sergeant, 0, 0,player);
    Pieces flagE(0, Flag, 3, 3,opponent);
    game->addKnown(&flagE,player);
    //dist=1
    newpos={3,2};
    BOOST_CHECK(ai->heuristicCalculator(&sergeant,newpos,game)==110);
    //dist=0
    newpos={3,3};
    BOOST_CHECK(ai->heuristicCalculator(&sergeant,newpos,game)==210);

    //case mypiece<ennemy
    Pieces marshalE(10, Marshal, 10, 10,opponent);
    game->addKnown(&marshalE,player);
    //dist=1
    newpos={10,9};
    BOOST_CHECK(ai->heuristicCalculator(&sergeant,newpos,game)==-40);
    //dist=0
    newpos={10,10};
    BOOST_CHECK(ai->heuristicCalculator(&sergeant,newpos,game)==-90);

    //case spy vs marshal
    Pieces spy(2, Spy, 0, 0,player);

    newpos={10,10};
    BOOST_CHECK(ai->heuristicCalculator(&spy,newpos,game)==510);
}

BOOST_AUTO_TEST_CASE(testGetPlayablePieces) {
    HeuristicAI* ai=new HeuristicAI();
    Game* game=new Game();
    auto player=game->getCurrentPlayer();
    Pieces piece(1, Scout, 0, 0,player);
    Pieces flag(0, Flag, 0, 0,player);
    game->addPiece(&piece,player);
    game->addPiece(&flag,player);
    BOOST_CHECK(ai->getPlayablePieces(game).size()==1);
    BOOST_CHECK(ai->getPlayablePieces(game)[0]==&piece);
}

BOOST_AUTO_TEST_SUITE_END()