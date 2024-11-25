#include <iostream>

// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}
// end of test SFML

#include <state.h>

using namespace std;
using namespace state;

int main() {
    auto game = Game::getInstance();
    game->startGame();
}

/*int main(int argc,char* argv[])
{
    cout << "It works !" << endl;
    return 0;
}*/

//
// Created by matthieu on 19/10/24.
//
//#include <iostream>
/*#include "state/Game.h"
#include "state/Player.h"
#include "state/Board.h"

using namespace std;
using namespace state;
int main() {
    auto game= Game::getInstance();
    auto board = Board::getInstance();
    auto piece1 = new Pieces(6, "Spy", 4, 0);
    auto piece2 = new Pieces(2, "Marshal", 0, 1);
    auto piece3 = new Pieces(3, "General", 0, 2);
    auto piece4 = new Pieces(4, "Colonel", 1, 4);
    auto piece5 = new Pieces(5, "Major", 0, 4);

    auto piece6 = new Pieces(6, "Captain", 5, 0);
    auto piece7 = new Pieces(7, "Lieutenant", 9, 1);
    auto piece8 = new Pieces(8, "Sergeant", 9, 2);
    auto piece9 = new Pieces(9, "Miner", 9, 3);
    auto piece10 = new Pieces(10, "Scout", 9, 4);

    auto player1 = game->getPlayer1();
    auto player2 = game->getPlayer2();

    player1->addPiece(piece1);
    player1->addPiece(piece2);
    player1->addPiece(piece3);
    player1->addPiece(piece4);
    player1->addPiece(piece5);

    player2->addPiece(piece6);
    player2->addPiece(piece7);
    player2->addPiece(piece8);
    player2->addPiece(piece9);
    player2->addPiece(piece10);

    for(auto piece: player1->getMyPieces())
    {
        board->setPieceOnBoard(piece);
    }

    for(auto piece: player2->getMyPieces())
    {
        board->setPieceOnBoard(piece);
    }
    game->startGame();
    return 0;
 }*/

