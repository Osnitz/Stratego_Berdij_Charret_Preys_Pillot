//
// Created by plt on 14/10/24.
//

#include "state.h"
#include "Pieces.h"
#include <iostream>
#include <utility>

using namespace std;
using namespace state;
using namespace engine;

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

int Pieces::getValue() {
    return value;
}

PieceType Pieces::getType() {
    return type;
}

int Pieces::getRange() {
    return range;
}

void Pieces::setCoord(const pair<int, int> &position) {
  	Board *board = Board::getInstance();
    int newx = position.first;
    int newy = position.second;
    board->removeFromBoard(this);
    this->x = newx;
    this->y = newy;
    board->setPieceOnBoard(this);
    cout << type << " was moved to (" << newx << ", " << newy << ").\n" << endl;
}