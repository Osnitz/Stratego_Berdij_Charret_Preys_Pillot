//
// Created by matthieu on 10/11/24.
//

#include <boost/test/included/unit_test.hpp>
#include "state.h"

using namespace state;

BOOST_AUTO_TEST_SUITE(PlayerTestSuite)

BOOST_AUTO_TEST_CASE(testGetters) {
    Player* player=new Player(0);
    Pieces piece(1, Scout, 0, 0,player);
    BOOST_CHECK(player->getPlayerID()==0);
    BOOST_CHECK(player->getMyPieces().empty());
    BOOST_CHECK(player->getCaptured().empty());
    BOOST_CHECK(player->getKnown().empty());
}

BOOST_AUTO_TEST_CASE(testDisplayCaptured) {
    Player* player=new Player(0);
    Pieces piece(2, Scout, 0, 0,player);
    Pieces flag(2, Flag, 0, 0,player);
    Pieces spy(2, Spy, 0, 0,player);
    Pieces miner(2, Miner, 0, 0,player);
    Pieces sergeant(2, Sergeant, 0, 0,player);
    Pieces lieutenant(2, Lieutenant, 0, 0,player);
    Pieces captain(2, Captain, 0, 0,player);
    Pieces major(2, Major, 0, 0,player);
    Pieces colonel(2, Colonel, 0, 0,player);
    Pieces general(2, General, 0, 0,player);
    Pieces marshal(2, Marshal, 0, 0,player);
    Pieces bomb(2, Bomb, 0, 0,player);

    Game* game=new Game();
    BOOST_TEST_MESSAGE("DisplayCaptured first output:");
    player->displayCaptured();

    game->addCaptured(&piece,player);
    game->addCaptured(&flag,player);
    game->addCaptured(&spy,player);
    game->addCaptured(&miner,player);
    game->addCaptured(&sergeant,player);
    game->addCaptured(&lieutenant,player);
    game->addCaptured(&captain,player);
    game->addCaptured(&major,player);
    game->addCaptured(&colonel,player);
    game->addCaptured(&general,player);
    game->addCaptured(&marshal,player);
    game->addCaptured(&bomb,player);
    BOOST_TEST_MESSAGE("DisplayCaptured  second output:");
    player->displayCaptured();
}

BOOST_AUTO_TEST_SUITE_END()
