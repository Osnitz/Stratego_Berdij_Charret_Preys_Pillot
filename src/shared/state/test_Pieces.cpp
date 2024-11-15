//
// Created by plt on 13/11/24.
//
#include <iostream>
#include "Player.h"
#include "Pieces.h"

using namespace std;
using namespace state;

void testConstructor() {
    Pieces piece(1, "Soldat", 0, 0);
    Pieces miner(2, "miner", 0, 0);
    Pieces flag(0, "flag", 0, 0);
    int x=piece.getPosition().first;
    int y=piece.getPosition().second;
    if(piece.getValue()==1&&piece.getName()=="Soldat"&&x==0&&y==0&&piece.getRange()==1&&miner.getRange()==10&&flag.getRange()==0) {
        std::cout << "Contructor test passed!" << std::endl;
    }else {
        std::cerr << "Constructor test failed!" << std::endl;
    }
}

void testCheckBoard() {
    Pieces piece(1, "Soldat", 0, 0);
    pair<int, int> outOfBoard={-1,10};
    pair<int, int> inTheLake={2,4};
    pair<int, int> goodPosition={1,4};
    if(!piece.CheckBoard(outOfBoard)&&!piece.CheckBoard(inTheLake)&&piece.CheckBoard(goodPosition)) {
        cout << "CheckBoard test passed!" << endl;
    }else {
        cerr << "CheckBoard test failed!" << endl;
    }
}

void testCheckRange() {
    Pieces piece(1, "Soldat", 0, 0);
    Pieces miner(2, "miner", 0, 0);
    Pieces flag(0, "flag", 0, 0);
    pair<int, int> goodrange={1,0};
    pair<int, int> othergoodrange={0,1};
    pair<int, int> minerrange={2,0};

    if(piece.CheckRange(goodrange)&&piece.CheckRange(othergoodrange)&&!piece.CheckRange(minerrange)&&miner.CheckRange(minerrange)&&!flag.CheckRange(goodrange)) {
        std::cout << "CheckRange test passed!" << std::endl;
    }else {
        std::cerr << "CheckRange test failed!" << std::endl;
    }
}

void testSetPosition() {
    Pieces piece(1, "Soldat", 0, 0);
    pair<int, int> newPosition={1,0};
    piece.setPosition(newPosition);
    if(piece.getPosition()==newPosition) {
        std::cout << "setPosition test passed!" << std::endl;
    }else {
        std::cerr << "setPosition test failed!" << std::endl;
    }
}

int main() {
    testConstructor();
    testCheckBoard();
    testCheckRange();
    testSetPosition();
    return 0;
}