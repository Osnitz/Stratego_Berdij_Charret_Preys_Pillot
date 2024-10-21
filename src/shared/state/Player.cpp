//
// Created by estelle on 14/10/24.
//

#include "Player.h"
#include "Pieces.h"
Player :: Player() {

}

Player:: ~Player(){};

void Player:: remove(Pieces * piece) {
    std::pair<int,int> positions=piece.getPosition();
    for(int i=0;i<myPieces.size();i++) {
        std::pair<int,int> mypos=myPieces[i].getPosition();
        if(positions[0]==mypos[0]&&positions[1]==mypos[1]) {
            myPieces.erase(i);
            exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_FAILURE);
}

void Player:: add(Pieces *piece) {
    int size=capturedPieces.size();
    int value=piece.getvalue();
    for(int i=0;i<size;i++) {
        int myvalue=capturedPieces[i].getvalue();
        if(value<=myvalue) {
            capturedPieces.insert(i,piece);
            return;
        }
    }
    capturedPieces.pushback(piece);
    return;
}

void Player::choose(Pieces *piece){}