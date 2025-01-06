//
// Created by matthieu on 06/01/25.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "client.h"
#include "state.h"

using namespace client;
using namespace state;

int main(void)
{
    printf("hello\n");
    auto game = new Game();
    auto pieces = new Pieces(2, PieceType::Scout, 0, 0, game-> getPlayer1());
    game->setPieceOnBoard(pieces, 0, 0);
    Render render(60,game); // Fenêtre 800x600, cases de 60 pixels
    render.run(game);
    return 0;

}