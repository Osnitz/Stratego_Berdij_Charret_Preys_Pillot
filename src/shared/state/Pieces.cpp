//
// Created by plt on 14/10/24.
//

#include "state.h"
#include "engine.h"
#include <iostream>
#include <random>
#include <utility>

using namespace std;
using namespace state;
using namespace engine;


auto game = Game::getInstance();

Pieces::Pieces(const int value, const PieceType type, int x, int y, bool color)
        : color(color), type(type), value(value), x(x), y(y) {

    if (type == PieceType::Scout) {
        range = 10;
    } else if (type == PieceType::Bomb || type == PieceType::Flag) {
        range = 0;
    } else {
        range = 1;
    }
}

Pieces::~Pieces() = default;

pair<int, int> Pieces::getPosition() {
    return {x, y};
}

int Pieces::getValue()  {
    return value;
}

PieceType Pieces::getType() {
    return type;
}

int Pieces::getRange()  {
    return range;
}

bool Pieces::LimitBoard(const pair<int, int>& &position, bool silent){
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

bool Pieces::isEmpty (Pieces * targetPiece) {
    if (targetPiece == nullptr) {
        return true;
    }
    return false;
}

bool Pieces::IsAlly(Pieces *targetPiece) {
    if(color==targetPiece->color) {
        return true;
    }
    return false;
}

bool Pieces::isEnemy(Pieces *targetPiece) {
    if(color!=targetPiece->color) {
        return true;
    }
    return false;
}

void Pieces::setPosition(const pair<int, int> &position) {
  	Board *board = Board::getInstance();
    int newx = position.first;
    int newy = position.second;
    board->removeFromBoard(this);
    this->x = newx;
    this->y = newy;
    board->setPieceOnBoard(this);
    cout << type << " was moved to (" << newx << ", " << newy << ").\n" << endl;
}

void Pieces::attack(const pair<int, int> &position) {
	Board *board = Board::getInstance();
    Pieces *attackedPiece = board->getPiece(position);
    auto player = game->getCurrentPlayer();
    auto opponent = game->getOpponent();

    if (attackedPiece == nullptr) {
        cerr << "No target found" << endl;
        return;
    }

    if (attackedPiece->type == PieceType::Bomb) {
        if (type == PieceType::Miner) {
            cout << "Good job ! The bomb is no more.\n" << endl;
            game->addCaptured(attackedPiece, player);
            setPosition(position);
            game->removePiece(attackedPiece, opponent);
            return;
        } else {
            cout << "Rest well ! The war is over for you.\n" << endl;
            game->addCaptured(this, opponent);
            board->removeFromBoard(this);
            game->removePiece(this, player);
            return;
        }
    }

    if (attackedPiece->type == PieceType::Marshal && type == PieceType::Spy) {
        cout << "Well done sir ! Their leader is gone.\n" << endl;
        game->addCaptured(attackedPiece, player);
        setPosition(position);
        game->removePiece(attackedPiece, opponent);
        return;
    }


    if (value > attackedPiece->value) {
        cout << "The enemy is down ! It was a " << attackedPiece->type << ".\n" << endl;
        game->addCaptured(attackedPiece, player);
        setPosition(position);
        game->removePiece(attackedPiece, opponent);
    }
    else if (value < attackedPiece->value) {
        cout << "The enemy is too strong ! It was a " << attackedPiece->type << ".\n" << endl;
        board->removeFromBoard(this);
        game->removePiece(this, player);
        game->addCaptured(this, opponent);
    }
    else {
        cout << "It's a tie ! It was a " << attackedPiece->type << " too.\n" << endl;
        game->addCaptured(attackedPiece, player);
        game->addCaptured(this, opponent);
        board->removeFromBoard(this);
        board->removeFromBoard(attackedPiece);
        game->removePiece(this, player);
        game->removePiece(attackedPiece, opponent);

    }
}





