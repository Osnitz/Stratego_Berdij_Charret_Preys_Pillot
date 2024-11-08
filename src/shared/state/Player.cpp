//
// Created by estelle on 14/10/24.
//

#include "Player.h"
#include "Pieces.h"
#include <iostream>

using namespace state;


Player :: Player() {
    std::vector<Pieces*> myPieces;
    bool isTurn;
    std::vector<Pieces*>  capturedPieces;
    std::vector<Pieces*>  knownPieces;
}

Player:: ~Player(){};

void Player::removePiece(Pieces* piece) {
    for (std::size_t i = 0; i < myPieces.size(); i++) {
        if (myPieces[i] == piece) {
            myPieces.erase(myPieces.begin() + i);
            break;
        }
    }
}

void Player:: addCaptured(Pieces *piece) {
    int size=capturedPieces.size();
    int value=piece->getValue();
    for(int i=0;i<size;i++) {
        int myvalue=capturedPieces[i]->getValue();
        if(value<=myvalue) {
            capturedPieces.insert(capturedPieces.begin()+i,piece);
            return;
        }
    }
    capturedPieces.push_back(piece);
}

std::vector<Pieces*> Player:: getCaptured() {
    return capturedPieces;
}

bool Player::belongTo(Pieces* piece) {
    for (std::size_t i = 0; i < myPieces.size(); i++) {
        if (myPieces[i] == piece) {
            return true;
        }
    }
    return false;
}


void Player:: displayCaptured () {
    int size=capturedPieces.size();
    for(int i=0;i<size;i++) {
        std::cout << capturedPieces[i]->getName()<< std::endl;
    }
}