//
// Created by plt on 14/10/24.
//

#include "Pieces.h"
#include "Board.h"
#include "Player.h"

#include <iostream>

using namespace std;
using namespace state;

Board *board = Board::getInstance();

std::pair<int, int> state::Pieces::getPosition() {
    return {x, y};
}

int state::Pieces::getValue() {
    return value;
}

std::string state::Pieces::getName() {
    return name;
}

int state::Pieces::getRange() {
    return range;
}

bool CheckBoard(std::pair<int, int> position){
    int NewX = position.first;
    int NewY = position.second;
    if ((NewX < 0) || (NewY < 0) || (NewX > 9) || (NewY > 9)) {
        std::cout << "Out of bounds" << std::endl;
        return false;
    }
    if ((NewY == 4) || (NewY == 5)) {
        if ((NewX == 2) || (NewX == 3) || (NewX == 6) || (NewX == 7)) {
            std::cout << "You can't cross lakes !" << std::endl;
            return false;
        }
    }
    return true;
}

bool CheckRange(Pieces* myPiece, std::pair<int, int> position) {
    int x = myPiece->getPosition().first;
    int y = myPiece->getPosition().second;
    int Newx = position.first;
    int Newy = position.second;

    int range = myPiece->getRange();

    if ((std::abs(Newx - x) <= range && Newy == y) || (std::abs(Newy - y) <= range && Newx == x)) {
        return true;
    }
    return false;
}

/*void Pieces::CheckCase (std::pair<int,int> position,Board *board) {
    std::vector<std::vector<Player * >> grid=board->getGrid();
    if(grid[position.first][position.second]==NULL) {
        this->setPosition(position);
        return;
    }
    if(grid[position.first][position.second]==) {

    }
}*/

void state::Pieces::setPosition(const std::pair<int, int> &position) {
    int newx = position.first;
    int newy = position.second;
    this->x = newx;
    this->y = newy;
    std::cout << name << " was moved to (" << newx << ", " << newy << ")." << std::endl;
}

void state::Pieces::attack(std::pair<int, int> position, Pieces *myPiece, Player *player) {
    Pieces *attackedPiece = board->getPiece(position);

    if (attackedPiece == nullptr) {
        std::cout << "No target found" << std::endl;
        return;
    }

    if (attackedPiece->getName() == "Bomb") {
        if (this->getName() == "Miner") {
            std::cout << "Good job ! The bomb is no more. " << std::endl;
            player->addCaptured(attackedPiece);
            return;
        } else {
            std::cout << "Rest well ! The war is over for you. " << std::endl;
            player->removePiece(this);
            return;
        }
    }

    if (attackedPiece->getName() == "Marshal") {
        if (this->getName() == "Spy") {
            std::cout << "Well done sir ! Their leader is gone. " << std::endl;
            player->addCaptured(attackedPiece);
            return;
        } else {
            std::cout << "What a shame ! You really wanted to be hero, didn't you ? " << std::endl;
            player->removePiece(this);
            return;
        }
    }
    if (this->getValue() > attackedPiece->getValue()) {
        std::cout << "The enemy is down ! It was a " << attackedPiece->getName() << "." << std::endl;
        player->addCaptured(attackedPiece);
        return;
    } else if (this->getValue() < attackedPiece->getValue()) {
        std::cout << "The enemy is too strong ! It was a " << attackedPiece->getName() << "." << std::endl;
        player->removePiece(this);
        return;
    } else if (this->getValue() == attackedPiece->getValue()) {
        std::cout << "It's a tie ! It was a " << attackedPiece->getName() << " too." << std::endl;
        player->removePiece(this);
        player->addCaptured(attackedPiece);
        return;
    }
}

