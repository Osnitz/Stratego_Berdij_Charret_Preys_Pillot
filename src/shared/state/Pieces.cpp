//
// Created by plt on 14/10/24.
//

#include "Pieces.h"
#include "Board.h"

#include <iostream>

using namespace std;
using namespace state;

Board *board = Board::getInstance();

std::pair<int, int> state::Pieces::getPosition() {
    return {x, y};
}

int state::Pieces::getValue() const {
    return value;
}

std::string state::Pieces::getName() {
    return name;
}

void state::Pieces::Walk(Pieces* piece, const std::pair<int, int> &position) {
    int newx = position.first;
    int newy = position.second;

    piece = board->getPiece(position);

    if (piece->getName() == "Flag" || piece->getName() == "Bomb") {
        std::cout << "Objects can't move on their own. Go back to school !" << std::endl;
        return;
    } else if (piece->getName() == "Scout") {

    }
    else {
        if (!((std::abs(newx - this->x) == 1 && newy == this->y) || (std::abs(newy - this->y) == 1 && newx == this->x))) {
            std::cout << "This piece can only move 1 space in a straight line!" << std::endl;
            setPosition(position);
            return;
        }
         }
}

void CheckBoard(std::pair<int, int> position){
    int NewX = position.first;
    int NewY = position.second;
    if ((NewX < 0) || (NewY < 0) || (NewX > 9) || (NewY > 9)) {
        std::cout << "Out of bounds" << std::endl;
        return;
    }
    if ((NewY == 4) || (NewY == 5)) {
        if ((NewX == 2) || (NewX == 3) || (NewX == 6) || (NewX == 7)) {
            std::cout << "You can't cross lakes !" << std::endl;
            return;
        }
    }
}

void state::Pieces::setPosition(const std::pair<int, int> &position) {
    int newx = position.first;
    int newy = position.second;
    this->x = newx;
    this->y = newy;
    std::cout << name << " was moved to (" << newx << ", " << newy << ")." << std::endl;
}

void state::Pieces::attack(Pieces *myPiece, const std::pair<int, int> &position, Player *player) {
    Pieces *attackedPiece = board->getPiece(position);

    if (attackedPiece == nullptr) {
        std::cout << "No target found" << std::endl;
        return;
    }

    if (attackedPiece->getName() == "Bomb") {
        if (this->getName() == "Miner") {
            std::cout << "Good job ! The bomb is no more. " << std::endl;
            player->remove(attackedPiece);
            return;
        } else {
            std::cout << "Rest well ! The war is over for you. " << std::endl;
            player->remove(this);
            return;
        }
    }

    if (attackedPiece->getName() == "Marshal") {
        if (this->getName() == "Spy") {
            std::cout << "Well done sir ! Their leader is gone. " << std::endl;
            player->remove(attackedPiece);
            return;
        } else {
            std::cout << "What a shame ! You really wanted to be hero, didn't you ? " << std::endl;
            player->remove(this);
            return;
        }
    }
    if (this->getValue() > attackedPiece->getValue()) {
        std::cout << "The enemy is down ! It was a " << attackedPiece->getName() << "." << std::endl;
        player->remove(attackedPiece);
        return;
    } else if (this->getValue() < attackedPiece->getValue()) {
        std::cout << "The enemy is too strong ! It was a " << attackedPiece->getName() << "." << std::endl;
        player->remove(this);
        return;
    } else if (this->getValue() == attackedPiece->getValue()) {
        std::cout << "It's a tie ! It was a " << attackedPiece->getName() << " too." << std::endl;
        player->remove(this);
        player->remove(attackedPiece);
        return;
    }
}
