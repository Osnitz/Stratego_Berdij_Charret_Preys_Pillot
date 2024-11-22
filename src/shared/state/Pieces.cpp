//
// Created by plt on 14/10/24.
//

#include "Pieces.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
#include "Game.h"

using namespace std;
using namespace state;

Board *board = Board::getInstance();
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
            std::cout << "Out of bounds" << std::endl;
        }
        return false;
    }
    if ((NewX == 4) || (NewX == 5)) {
        if ((NewY == 2) || (NewY == 3) || (NewY == 6) || (NewY == 7)) {
            if (!silent)
            {
                std::cerr << "You can't cross lakes !" << std::endl;
            }
            return false;
        }
    }
    return true;
}

/*bool Pieces::CheckRange(std::pair<int, int> position) {
    int x = this->getPosition().first;
    int y = this->getPosition().second;
    int Newx = position.first;
    int Newy = position.second;

    int range = this->getRange();

    if ((abs(Newx - x) <= range && Newy == y) || (abs(Newy - y) <= range && Newx == x)) {
        return true;
    }
    return false;
}*/

string Pieces::CheckCase (pair<int,int> position) {
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
    Board* board = Board::getInstance();
    int newx = position.first;
    int newy = position.second;
    board->removeFromBoard(this);
    this->x = newx;
    this->y = newy;
    board->setPieceOnBoard(this);
    std::cout << name << " was moved to (" << newx << ", " << newy << ").\n" << std::endl;
}

void Pieces::attack(pair<int, int> position) {

    Pieces *attackedPiece = board->getPiece(position);
    auto player = game->getCurrentPlayer();

    if (attackedPiece == nullptr) {
        std::cerr << "No target found" << std::endl;
        return;
    }

    if (attackedPiece->getName() == "Bomb") {
        if (this->getName() == "Miner") {

            std::cout << "Good job ! The bomb is no more.\n" << std::endl;
            player->addCaptured(attackedPiece);
            setPosition(position);
            game->Graveyard = attackedPiece;
            return;
        } else {

            std::cout << "Rest well ! The war is over for you.\n" << std::endl;
            player->addCaptured(attackedPiece);
            board->removeFromBoard(this);
            player->removePiece(this);
            game->Purgatory = this;
            game->Graveyard = attackedPiece;
            return;
        }
    }

    if (attackedPiece->getName() == "Marshal" && getName() == "Spy") {
        std::cout << "Well done sir ! Their leader is gone.\n" << std::endl;
        player->addCaptured(attackedPiece);
        setPosition(position);
        Game::getInstance()->Graveyard = attackedPiece;
        return;
    }

    if (getValue() > attackedPiece->getValue()) {
        std::cout << "The enemy is down ! It was a " << attackedPiece->getName() << ".\n" << std::endl;
        player->addCaptured(attackedPiece);
        setPosition(position);
        game->Graveyard = attackedPiece;

    } else if (getValue() < attackedPiece->getValue()) {
        std::cout << "The enemy is too strong ! It was a " << attackedPiece->getName() << ".\n" << std::endl;
        board->removeFromBoard(this);
        player->removePiece(this);
        game->Purgatory = this;

    } else {
        std::cout << "It's a tie ! It was a " << attackedPiece->getName() << " too.\n" << std::endl;
        player->addCaptured(attackedPiece);
        board->removeFromBoard(this);
        player->removePiece(this);
        game->Purgatory = this;
        game->Graveyard = attackedPiece;
    }
}

std::vector<std::pair<int, int>> Pieces::canMove(Pieces* pieceToMove) {
    std::vector<std::pair<int, int>> possiblePositions;

    if (pieceToMove == nullptr) {
        return possiblePositions;
    }

    auto position = pieceToMove->getPosition();
    int x = position.first;
    int y = position.second;
    int range = pieceToMove->getRange();

    for (int i = 1; i <= range; ++i) {
        std::pair<int, int> posAbove = {x, y - i};
        if (CheckBoard(posAbove,true)) {
            std::string caseStatus = CheckCase(posAbove);
            if (caseStatus != "Ally") {
                possiblePositions.push_back(posAbove);
            }
            if (caseStatus != "Empty") {
                break;
            }
        }
    }

    for (int i = 1; i <= range; ++i) {
        std::pair<int, int> posBelow = {x, y + i};
        if (CheckBoard(posBelow,true)) {
            std::string caseStatus = CheckCase(posBelow);
            if (caseStatus != "Ally") {
                possiblePositions.push_back(posBelow);
            }
            if (caseStatus != "Empty") {
                break;
            }
        }
    }

    for (int i = 1; i <= range; ++i) {
        std::pair<int, int> posLeft = {x - i, y};
        if (CheckBoard(posLeft,true)) {
            std::string caseStatus = CheckCase(posLeft);
            if (caseStatus != "Ally") {
                possiblePositions.push_back(posLeft);
            }
            if (caseStatus != "Empty") {
                break;
            }
        }
    }

    for (int i = 1; i <= range; ++i) {
        std::pair<int, int> posRight = {x + i, y};
        if (CheckBoard(posRight,true)) {
            std::string caseStatus = CheckCase(posRight);
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



