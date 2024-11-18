//
// Created by estelle on 14/10/24.
//

#include "Player.h"
#include "Pieces.h"
#include "Board.h"
#include <iostream>

using namespace state;


Player :: Player() {
    std::vector<Pieces*> myPieces;
    std::vector<Pieces*>  capturedPieces= {nullptr};
    std::vector<Pieces*>  knownPieces;
}

Player:: ~Player(){};

void Player::removePiece(Pieces* piece) {
    if (myPieces.empty()) {
        std::cerr<<"No piece left"<<std::endl;
        return;
    }
    for (std::size_t i = 0; i < myPieces.size(); i++) {
        if (myPieces[i] == piece) {
            myPieces.erase(myPieces.begin() + i);
            return;
        }
    }
    std::cerr<<"Can't remove this piece : it doesn't exist!"<<std::endl;
}

void Player::addPiece(Pieces* piece) {
    myPieces.push_back(piece);
}

void Player:: addCaptured(Pieces *piece) {
    Board::getInstance()->removeFromBoard(piece);
    auto size=capturedPieces.size();
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

std::vector<Pieces*> Player:: getMyPieces() {
    return myPieces;
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
    if(capturedPieces.empty()) {
        std::cerr<<"You have no captured pieces"<<std::endl;
        return;
    }
    auto size=capturedPieces.size();
    for(int i=0;i<size;i++) {
        std::cout << capturedPieces[i]->getName()<< std::endl;
    }
}