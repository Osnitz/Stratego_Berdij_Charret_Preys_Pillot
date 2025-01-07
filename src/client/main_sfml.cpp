//
// Created by matthieu on 06/01/25.
//
#include <SFML/Graphics.hpp>
#include "client.h"
#include "state.h"


using namespace client;
using namespace state;


int main() {
    auto game = new Game();
    /*auto piece1 = new Pieces(2, PieceType::Scout, 0, 0, game-> getPlayer1());
    game->setPieceOnBoard(piece1, 0, 0);
    game->addPiece(piece1, game->getPlayer1());


    auto piece2 = new Pieces(2, PieceType::Scout, 9, 9, game-> getPlayer2());
    game->setPieceOnBoard(piece2, 9, 9);
    game->addPiece(piece2, game->getPlayer2());*/


    std::string filepath = "../src/shared/state/config/Balance.csv";
    game->loadConfig(filepath);
    game->switchTurn();
    game->loadConfig(filepath);
    game->switchTurn();


    Render render(60, game); // Fenêtre 800x600, cases de 60 pixels
    render.run();
    return 0;
}