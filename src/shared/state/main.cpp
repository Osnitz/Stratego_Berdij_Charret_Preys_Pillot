//
// Created by matthieu on 19/10/24.
//
#include <iostream>
//#include "Game.h"
#include "Board.h"

using namespace std;
using namespace state;
int main() {
    //Game game;
    Board *board = Board::getInstance();
    board->displayBoard();
    //game.startGame();

    return 0;
}
