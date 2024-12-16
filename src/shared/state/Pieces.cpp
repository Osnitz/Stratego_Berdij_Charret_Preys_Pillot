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

Pieces::Pieces(const int value, const PieceType type, int x, int y, Player* owner)
        : owner (owner), type(type), value(value), x(x), y(y) {

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

void Pieces::setCoord(int newx, int newy) {
    this->x = newx;
    this->y = newy;
    cout << type << " was moved to (" << newx << ", " << newy << ").\n" << endl;
}

Player *Pieces::getOwner() {
    return owner;
}
