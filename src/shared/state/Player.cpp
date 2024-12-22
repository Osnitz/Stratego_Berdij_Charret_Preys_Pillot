//
// Created by estelle on 14/10/24.
//

#include "state.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace state;
using namespace std;

Player :: Player(int PlayerID):PlayerID(PlayerID) {
    vector<Pieces *> myPieces;
    vector<Pieces *> capturedPieces = {nullptr};
    vector<Pieces *> knownPieces;

}

Player:: ~Player()= default;

std::vector<Pieces *> &Player::getCaptured() {
    return capturedPieces;
}

std::vector<Pieces *> &Player::getMyPieces() {
    return myPieces;
}

void Player:: displayCaptured () {
    if(capturedPieces.empty()) {
        cerr<<"You have no captured pieces"<<endl;
        return;
    }
    auto size=capturedPieces.size();
    for(size_t i=0;i<size;i++) {
        cout << capturedPieces[i]->getType()<< endl;
    }
}

int Player::getPlayerID() {
    return PlayerID;
}