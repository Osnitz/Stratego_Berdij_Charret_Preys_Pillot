//
// Created by matthieu on 10/11/24.
//
// #include <iostream>
// #include "state.h"
//
// using namespace state;
//
// void testAddPiece() {
//     Player player;
//     Pieces piece(1, "Soldat", 0, 0);
//     player.addPiece(&piece);
//     if (player.getMyPieces().size() == 1 && player.getMyPieces()[0] == &piece) {
//         std::cout << "AddPiece test passed!" << std::endl;
//     } else {
//         std::cerr << "AddPiece test failed!" << std::endl;
//     }
// }
//
// void testRemovePiece() {
//     Player player;
//     Pieces piece(1, "Soldat", 0, 0);
//     Pieces notmypiece(1, "Soldat", 1, 1);
//     player.addPiece(&piece);
//     player.removePiece(&notmypiece); //we must get the message : Can't remove this piece : it doesn't exist!
//     player.removePiece(&piece);
//     if (player.getMyPieces().empty()) {
//         std::cout << "RemovePiece test passed!" << std::endl;
//     } else {
//         std::cerr << "RemovePiece test failed!" << std::endl;
//     }
//     player.removePiece(&piece); //we must get the message : No piece left
// }
//
// void testAddCaptured() {
//     Player player;
//     Pieces piece1(1, "Soldat", 0, 0);
//     Pieces piece2(2, "Cavalier", 0, 0);
//     player.addCaptured(&piece2);
//     player.addCaptured(&piece1);
//     if (player.getCaptured().size() == 2 && player.getCaptured()[0] == &piece1 && player.getCaptured()[1] == &piece2) {
//         std::cout << "AddCaptured test passed!" << std::endl;
//     } else {
//         std::cerr << "AddCaptured test failed!" << std::endl;
//     }
// }
//
// void testBelongTo() {
//     Player player;
//     Pieces piece(1, "Soldat", 0, 0);
//     Pieces notmypiece(1, "Soldat", 1, 1);
//     player.addPiece(&piece);
//     if (player.belongTo(&piece)&&!player.belongTo(&notmypiece)) {
//         std::cout << "BelongTo test passed!" << std::endl;
//     } else {
//         std::cerr << "BelongTo test failed!" << std::endl;
//     }
// }
//
// void testDisplayCaptured() {
//     Player player;
//     std::cout << "DisplayCaptured test first output:" << std::endl;
//     player.displayCaptured();
//     Pieces piece(1, "Soldat", 0, 0);
//     player.addCaptured(&piece);
//     std::cout << "DisplayCaptured test second output:" << std::endl;
//     player.displayCaptured();
// }
//
// int main() {
//     testAddPiece();
//     testRemovePiece();
//     testAddCaptured();
//     testBelongTo();
//     testDisplayCaptured();
//     return 0;
// }

#define BOOST_TEST_MODULE PlayerTests
#include <boost/test/included/unit_test.hpp>
#include "state.h"

using namespace state;

BOOST_AUTO_TEST_SUITE(PlayerTestSuite)

BOOST_AUTO_TEST_CASE(testAddPiece) {
    Player player;
    Pieces piece(1, "Soldat", 0, 0);
    player.addPiece(&piece);

    BOOST_CHECK_EQUAL(player.getMyPieces().size(), 1);
    BOOST_CHECK(player.getMyPieces()[0] == &piece);
}

BOOST_AUTO_TEST_CASE(testRemovePiece) {
    Player player;
    Pieces piece(1, "Soldat", 0, 0);
    Pieces notmypiece(1, "Soldat", 1, 1);

    player.addPiece(&piece);
    player.removePiece(&notmypiece); // Doit afficher un message : "Can't remove this piece : it doesn't exist!"
    player.removePiece(&piece);

    BOOST_CHECK(player.getMyPieces().empty());

    player.removePiece(&piece); // Doit afficher un message : "No piece left"
}

BOOST_AUTO_TEST_CASE(testAddCaptured) {
    Player player;
    Pieces piece1(1, "Soldat", 0, 0);
    Pieces piece2(2, "Cavalier", 0, 0);

    player.addCaptured(&piece2);
    player.addCaptured(&piece1);

    BOOST_CHECK_EQUAL(player.getCaptured().size(), 2);
    BOOST_CHECK(player.getCaptured()[0] == &piece1);
    BOOST_CHECK(player.getCaptured()[1] == &piece2);
}

BOOST_AUTO_TEST_CASE(testBelongTo) {
    Player player;
    Pieces piece(1, "Soldat", 0, 0);
    Pieces notmypiece(1, "Soldat", 1, 1);

    player.addPiece(&piece);

    BOOST_CHECK(player.belongTo(&piece));
    BOOST_CHECK(!player.belongTo(&notmypiece));
}

BOOST_AUTO_TEST_CASE(testDisplayCaptured) {
    Player player;
    Pieces piece(1, "Soldat", 0, 0);

    BOOST_TEST_MESSAGE("DisplayCaptured test first output:");
    player.displayCaptured();

    player.addCaptured(&piece);
    BOOST_TEST_MESSAGE("DisplayCaptured test second output:");
    player.displayCaptured();
}

BOOST_AUTO_TEST_SUITE_END()
