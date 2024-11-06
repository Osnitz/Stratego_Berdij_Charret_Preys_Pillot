//
// Created by matthieu on 19/10/24.
//
#include <iostream>
//#include "Game.h"
#include "Player.h"
#include "Board.h"

using namespace std;
using namespace state;
int main() {
    //Game game;
    Board *board = Board::getInstance();
    Player *player1 = new Player();
    board->displayBoard(*player1);
    //game.startGame();
    return 0;
}
