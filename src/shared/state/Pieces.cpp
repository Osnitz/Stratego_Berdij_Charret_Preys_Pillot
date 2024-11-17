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

Pieces::Pieces(int value, std::string name, int x, int y) {
    this->value = value;
    this->name = name;
    this->range = 1;
    this->x = x;
    this->y = y;
}

Pieces::~Pieces() {
}

std::pair<int, int> state::Pieces::getPosition() {
    return {x, y};
}

int Pieces::getValue() {
    return value;
}

std::string Pieces::getName() {
    return name;
}

int Pieces::getRange() {
    return range;
}

bool Pieces::CheckBoard(std::pair<int, int> position){
    int NewX = position.first;
    int NewY = position.second;
    if ((NewX < 0) || (NewY < 0) || (NewX > 9) || (NewY > 9)) {
        //std::cout << "Out of bounds" << std::endl;
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

/*bool Pieces::CheckRange(std::pair<int, int> position) {
    int x = this->getPosition().first;
    int y = this->getPosition().second;
    int Newx = position.first;
    int Newy = position.second;

    int range = this->getRange();

    if ((std::abs(Newx - x) <= range && Newy == y) || (std::abs(Newy - y) <= range && Newx == x)) {
        return true;
    }
    return false;
}*/

string Pieces::CheckCase (std::pair<int,int> position) {
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

void Pieces::setPosition(const std::pair<int, int> &position) {
    Board* board = Board::getInstance();
    int newx = position.first;
    int newy = position.second;
    board->removeFromBoard(this);
    this->x = newx;
    this->y = newy;
    board->setPieceOnBoard(this);
    std::cout << name << " was moved to (" << newx << ", " << newy << ")." << std::endl;
}

void Pieces::attack(std::pair<int, int> position) {

    Pieces *attackedPiece = board->getPiece(position);
    auto player = game->getCurrentPlayer();

    if (attackedPiece == nullptr) {
        std::cout << "No target found" << std::endl;
        return;
    }

    if (attackedPiece->getName() == "Bomb") {
        if (this->getName() == "Miner") {
            std::cout << "Good job ! The bomb is no more. " << std::endl;
            player->addCaptured(attackedPiece);
            setPosition(position);
            game->Graveyard = attackedPiece;
            return;
        } else {
            std::cout << "Rest well ! The war is over for you. " << std::endl;
            player->addCaptured(attackedPiece);
            board->removeFromBoard(this);
            player->removePiece(this);
            game->Purgatory = this;
            game->Graveyard = attackedPiece;
            return;
        }
    }

    if (attackedPiece->getName() == "Marshal" && getName() == "Spy") {
        std::cout << "Well done sir ! Their leader is gone. " << std::endl;
        player->addCaptured(attackedPiece);
        setPosition(position);
        Game::getInstance()->Graveyard = attackedPiece;
        return;
    }

    if (getValue() > attackedPiece->getValue()) {
        std::cout << "The enemy is down ! It was a " << attackedPiece->getName() << "." << std::endl;
        player->addCaptured(attackedPiece);
        setPosition(position);
        game->Graveyard = attackedPiece;

    } else if (getValue() < attackedPiece->getValue()) {
        std::cout << "The enemy is too strong ! It was a " << attackedPiece->getName() << "." << std::endl;
        board->removeFromBoard(this);
        player->removePiece(this);
        game->Purgatory = this;

    } else {
        std::cout << "It's a tie ! It was a " << attackedPiece->getName() << " too." << std::endl;
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
        if (CheckBoard(posAbove)) {
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
        if (CheckBoard(posBelow)) {
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
        if (CheckBoard(posLeft)) {
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
        if (CheckBoard(posRight)) {
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



