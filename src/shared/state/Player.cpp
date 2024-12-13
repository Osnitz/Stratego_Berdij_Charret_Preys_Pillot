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

Player :: Player(): color(false) {
    vector<Pieces *> myPieces;
    vector<Pieces *> capturedPieces = {nullptr};
    vector<Pieces *> knownPieces;
}

Player:: ~Player()= default;

void Player::loadConfig(const string& fileName){
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

        auto type = static_cast<PieceType>(stoi(dataline.at(0)));
        int value = stoi(dataline.at(1));
        int x = stoi(dataline.at(2));
        int y = stoi(dataline.at(3));

        if (game->getCurrentPlayer() == game->getPlayer1()) {
            auto * piece = new Pieces(value, type, x, y, true);
            myPieces.push_back(piece);
            board->setPieceOnBoard(piece);
        }
        else {
            auto * piece = new Pieces(value, type, 9 - x, y, false);
            myPieces.push_back(piece);
            board->setPieceOnBoard(piece);
        }
    }
    board->displayBoard(*game->getCurrentPlayer());
}

void Player::addPiece(Pieces* piece) {
    myPieces.push_back(piece);
}

std::vector<Pieces *> &Player::getCaptured() {
    return capturedPieces;
}

std::vector<Pieces *> &Player::getMyPieces() {
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
        cout << capturedPieces[i]->getType()<< endl;
    }
}
