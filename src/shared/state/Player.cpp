//
// Created by estelle on 14/10/24.
//

#include "state.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace state;
using namespace std;

Player :: Player(int PlayerID) : PlayerID(PlayerID){
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
        switch (capturedPieces[i]->getType()) {
            case Flag:cout << "Flag"<< endl; break;
            case Spy:cout << "Spy"<< endl; break;
            case Scout:cout << "Scout"<< endl; break;
            case Miner:cout << "Miner"<< endl; break;
            case Sergeant:cout << "Sergeant"<< endl; break;
            case Lieutenant:cout << "Lieutenant"<< endl; break;
            case Captain:cout << "Captain"<< endl; break;
            case Major:cout << "Major"<< endl; break;
            case Colonel:cout << "Colonel"<< endl; break;
            case General:cout << "General"<< endl; break;
            case Bomb:cout<<"Bomb"<<endl;break;
            default:cout << "Marshal"<< endl; break;
        }
    }
}

int Player::getPlayerID() {
    return PlayerID;
}