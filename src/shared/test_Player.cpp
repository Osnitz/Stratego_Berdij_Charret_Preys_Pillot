//
// Created by matthieu on 10/11/24.
//
#include <iostream>
#include "state.h"
#include "engine.h"

using namespace state;

void testAddPiece() {
    Player player;
    Pieces piece(1, "Soldat", 0, 0);
    player.addPiece(&piece);
    if (player.getMyPieces().size() == 1 && player.getMyPieces()[0] == &piece) {
        std::cout << "AddPiece test passed!" << std::endl;
    } else {
        std::cerr << "AddPiece test failed!" << std::endl;
    }
}

void testRemovePiece() {
    Player player;
    Pieces piece(1, "Soldat", 0, 0);
    Pieces notmypiece(1, "Soldat", 1, 1);
    player.addPiece(&piece);
    player.removePiece(&notmypiece); //we must get the message : Can't remove this piece : it doesn't exist!
    player.removePiece(&piece);
    if (player.getMyPieces().empty()) {
        std::cout << "RemovePiece test passed!" << std::endl;
    } else {
        std::cerr << "RemovePiece test failed!" << std::endl;
    }
    player.removePiece(&piece); //we must get the message : No piece left
}

void testAddCaptured() {
    Player player;
    Pieces piece1(1, "Soldat", 0, 0);
    Pieces piece2(2, "Cavalier", 0, 0);
    player.addCaptured(&piece2);
    player.addCaptured(&piece1);
    if (player.getCaptured().size() == 2 && player.getCaptured()[0] == &piece1 && player.getCaptured()[1] == &piece2) {
        std::cout << "AddCaptured test passed!" << std::endl;
    } else {
        std::cerr << "AddCaptured test failed!" << std::endl;
    }
}

void testBelongTo() {
    Player player;
    Pieces piece(1, "Soldat", 0, 0);
    Pieces notmypiece(1, "Soldat", 1, 1);
    player.addPiece(&piece);
    if (player.belongTo(&piece)&&!player.belongTo(&notmypiece)) {
        std::cout << "BelongTo test passed!" << std::endl;
    } else {
        std::cerr << "BelongTo test failed!" << std::endl;
    }
}

void testDisplayCaptured() {
    Player player;
    std::cout << "DisplayCaptured test first output:" << std::endl;
    player.displayCaptured();
    Pieces piece(1, "Soldat", 0, 0);
    player.addCaptured(&piece);
    std::cout << "DisplayCaptured test second output:" << std::endl;
    player.displayCaptured();
}

int main() {
    testAddPiece();
    testRemovePiece();
    testAddCaptured();
    testBelongTo();
    testDisplayCaptured();
    return 0;
}