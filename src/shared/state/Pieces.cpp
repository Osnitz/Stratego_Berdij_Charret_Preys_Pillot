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

bool Pieces::CheckBoard(pair<int, int> position){
    int NewX = position.first;
    int NewY = position.second;
    if ((NewX < 0) || (NewY < 0) || (NewX > 9) || (NewY > 9)) {
        cout << "Out of bounds !" << endl;
        return false;
    }
    if ((NewY == 4) || (NewY == 5)) {
        if ((NewX == 2) || (NewX == 3) || (NewX == 6) || (NewX == 7)) {
            cout << "You can't cross lakes !" << endl;
            return false;
        }
    }
    return true;
}

bool Pieces::CheckRange(std::pair<int, int> position) {
    int x = this->getPosition().first;
    int y = this->getPosition().second;
    int Newx = position.first;
    int Newy = position.second;

    int range = this->getRange();

    if ((abs(Newx - x) <= range && Newy == y) || (abs(Newy - y) <= range && Newx == x)) {
        return true;
    }
    return false;
}

string Pieces::CheckCase (pair<int,int> position) {
    Pieces *targetPiece = board->getPiece(position);
    Player* currentPlayer = game->getCurrentPlayer();

    if (targetPiece == nullptr) {
        this->setPosition(position);
        return "Empty";
    }
    if (!currentPlayer->belongTo(targetPiece)) {
        this->attack(position);
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
    cout << name << " was moved to (" << newx << ", " << newy << ")." << endl;
}

void Pieces::attack(pair<int, int> position) {

    Pieces *attackedPiece = board->getPiece(position);
    auto player = game->getCurrentPlayer();

    if (attackedPiece == nullptr) {
        cout << "No target found" << endl;
        return;
    }

    if (attackedPiece->getName() == "Bomb") {
        if (this->getName() == "Miner") {
            cout << "Good job ! The bomb is no more. " << endl;
            player->addCaptured(attackedPiece);
            this->setPosition(position);
            game->Graveyard = attackedPiece;
            return;
        } else {
            cout << "Rest well ! The war is over for you. " << endl;
            player->addCaptured(attackedPiece);
            board->removeFromBoard(this);
            player->removePiece(this);
            game->Purgatory = this;
            game->Graveyard = attackedPiece;
            return;
        }
    }

    if (attackedPiece->getName() == "Marshal") {
        if (this->getName() == "Spy") {
            std::cout << "Well done sir ! Their leader is gone. " << std::endl;
            player->addCaptured(attackedPiece);
            this->setPosition(position);
            game->Graveyard = attackedPiece;
            return;
        }
    }

    if (this->getValue() > attackedPiece->getValue()) {
        std::cout << "The enemy is down ! It was a " << attackedPiece->getName() << "." << std::endl;
        player->addCaptured(attackedPiece);
        this->setPosition(position);
        game->Graveyard = attackedPiece;
        return;
    } else if (this->getValue() < attackedPiece->getValue()) {
        std::cout << "The enemy is too strong ! It was a " << attackedPiece->getName() << "." << std::endl;
        board->removeFromBoard(this);
        player->removePiece(this);
        game->Purgatory = this;
        return;
    } else if (this->getValue() == attackedPiece->getValue()) {
        std::cout << "It's a tie ! It was a " << attackedPiece->getName() << " too." << std::endl;
        player->addCaptured(attackedPiece);
        board->removeFromBoard(this);
        player->removePiece(this);
        game->Purgatory = this;
        game->Graveyard = attackedPiece;
        return;
    }
}

