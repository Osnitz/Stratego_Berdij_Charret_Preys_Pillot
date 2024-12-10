//
// Created by estelle on 14/10/24.
//

#include "state.h"
#include "engine.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace state;
using namespace std;

Player :: Player() {
    vector<Pieces*> myPieces;
    vector<Pieces*>  capturedPieces= {nullptr};
    vector<Pieces*>  knownPieces;
}

Player:: ~Player(){};


void Player::loadConfig(string fileName){
    Game *game = Game::getInstance();
    Board *board = Board::getInstance();
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "File " << fileName << " not found " << endl;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        cout << line << endl;
        stringstream ss(line);
        string cell;
        vector<string> dataline;

        while (getline(ss, cell, ',')) {
            cell.erase(remove(cell.begin(), cell.end(), '"'), cell.end());
            dataline.push_back(cell);
        }

        std::string name = dataline.at(0);
        int value = stoi(dataline.at(1));
        int x = stoi(dataline.at(2));
        int y = stoi(dataline.at(3));


        if (game->getCurrentPlayer() == game->getPlayer1()) {
            Pieces* piece = new Pieces(value, name, x, y);
            myPieces.push_back(piece);
            board->setPieceOnBoard(piece);
        }
        else {
            Pieces* piece = new Pieces(value, name, 9 - x, y);
            myPieces.push_back(piece);
            board->setPieceOnBoard(piece);
        }
    }
    board->displayBoard(*game->getCurrentPlayer());

}



void Player::addPiece(Pieces* piece) {
    myPieces.push_back(piece);
}



vector<Pieces*> Player:: getCaptured() {
    return capturedPieces;
}

vector<Pieces*> Player:: getMyPieces() {
    return myPieces;
}

bool Player::belongTo(Pieces* piece) {
    for (size_t i = 0; i < myPieces.size(); i++) {
        if (myPieces[i] == piece) {
            return true;
        }
    }
    return false;
}

void Player:: displayCaptured () {
    if(capturedPieces.empty()) {
        cerr<<"You have no captured pieces"<<endl;
        return;
    }
    auto size=capturedPieces.size();
    for(size_t i=0;i<size;i++) {
        cout << capturedPieces[i]->getName()<< endl;
    }
}