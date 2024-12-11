//
// Created by plt on 14/10/24.
//

#include "state.h"
#include "engine.h"
#include <iostream>
#include <random>

using namespace std;
using namespace state;
using namespace engine;


auto game = Game::getInstance();

Pieces::Pieces(int value, string name, int x, int y) {
    this->value = value;
    this->name = name;
    if(value==2) {
        this->range = 10;
    }else if(value==11||value==0) {
        this->range = 0;
    }
    else{this->range=1;}
    this->x = x;
    this->y = y;
}

Pieces::~Pieces() {
}

pair<int, int> Pieces::getPosition() {
    return {x, y};
}

int Pieces::getValue() {
    return value;
}

string Pieces::getName() {
    return name;
}

int Pieces::getRange() {
    return range;
}

bool Pieces::CheckBoard(pair<int, int> position,bool silent){
    int NewX = position.first;
    int NewY = position.second;
    if ((NewX < 0) || (NewY < 0) || (NewX > 9) || (NewY > 9)) {
        if (!silent)
        {
            cerr << "Out of bounds" << endl;
        }
        return false;
    }
    if ((NewX == 4) || (NewX == 5)) {
        if ((NewY == 2) || (NewY == 3) || (NewY == 6) || (NewY == 7)) {
            if (!silent)
            {
                cerr << "You can't cross lakes !" << endl;
            }
            return false;
        }
    }
    return true;
}

string Pieces::CheckCase (pair<int,int> position) {
  	Board *board = Board::getInstance();
    Pieces *targetPiece = board->getPiece(position);
    Player* currentPlayer = game->getCurrentPlayer();

    if (targetPiece == nullptr) {
        return "Empty";
    }
    if (!currentPlayer->belongTo(targetPiece)) {
        return "Enemy";
    }

    return "Ally";
}

void Pieces::setPosition(const pair<int, int> &position) {
  	Board *board = Board::getInstance();
    int newx = position.first;
    int newy = position.second;
    board->removeFromBoard(this);
    this->x = newx;
    this->y = newy;
    board->setPieceOnBoard(this);
    cout << name << " was moved to (" << newx << ", " << newy << ").\n" << endl;
}

vector<pair<int, int>> Pieces::canMove(Pieces* pieceToMove) {
    vector<pair<int, int>> possiblePositions;

    if (pieceToMove == nullptr) {
        return possiblePositions;
    }

    int x = pieceToMove->x;
    int y = pieceToMove->y;
    int range = pieceToMove->range;

    for (int i = 1; i <= range; ++i) {
        pair<int, int> posAbove = {x, y - i};
        if (CheckBoard(posAbove,true)) {
            string caseStatus = CheckCase(posAbove);
            if (caseStatus != "Ally") {
                possiblePositions.push_back(posAbove);
            }
            if (caseStatus != "Empty") {
                break;
            }
        }
    }

    for (int i = 1; i <= range; ++i) {
        pair<int, int> posBelow = {x, y + i};
        if (CheckBoard(posBelow,true)) {
            string caseStatus = CheckCase(posBelow);
            if (caseStatus != "Ally") {
                possiblePositions.push_back(posBelow);
            }
            if (caseStatus != "Empty") {
                break;
            }
        }
    }

    for (int i = 1; i <= range; ++i) {
        pair<int, int> posLeft = {x - i, y};
        if (CheckBoard(posLeft,true)) {
            string caseStatus = CheckCase(posLeft);
            if (caseStatus != "Ally") {
                possiblePositions.push_back(posLeft);
            }
            if (caseStatus != "Empty") {
                break;
            }
        }
    }

    for (int i = 1; i <= range; ++i) {
        pair<int, int> posRight = {x + i, y};
        if (CheckBoard(posRight,true)) {
            string caseStatus = CheckCase(posRight);
            if (caseStatus != "Ally") {
                possiblePositions.push_back(posRight);
            }
            if (caseStatus != "Empty") {
                break;
            }
        }
    }

    return possiblePositions;
}



