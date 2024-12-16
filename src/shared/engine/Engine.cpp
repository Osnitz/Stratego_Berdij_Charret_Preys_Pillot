//
// Created by plt on 11/12/24.
//
#include <iostream>
#include <state/Pieces.h>
#include <state/PieceType.h>

#include "../engine.h"

using namespace engine;
using namespace state;
using namespace std;

Engine::Engine(Game* game, vector<Player*> players)
    : game(game), players(players)
{
}


void Engine::startGame()
{
    currentPlayerIndex = 0; // Commence avec le premier joueur
    game->switchTurn();
    std::cout << "Game started. Entering Placement Phase." << endl;
}


void Engine::endTurn()
{
    game->switchTurn();
}

bool Engine::handleCmdPlacement(string filePath)
{
    auto currentPlayer = game->currentPlayer;
    currentPlayer->loadConfig(filePath);
    endTurn();
    return true;
}


bool Engine::handleCmdMove(pair<int, int> from, pair<int, int> to)
{
    auto board = Board::getInstance();
    auto mypiece = board->getPiece(from);
    auto currentplayer = game->currentPlayer;
    if (!currentplayer->belongTo(mypiece))
    {
        cerr << "This is not your piece !" << endl;
        return false;
    }
    if (!isValidMove(mypiece, to))
    {
        cerr << "Invalid destination" << endl;
        return false;
    }
    auto targetpiece = board->getPiece(to);
    if (mypiece->isEmpty(targetpiece))
    {
        mypiece->setPosition(to);
        endTurn();
        return true;
    }
    attack(mypiece, to);
    endTurn();
    return true;
}

void Engine::attack(Pieces* mypiece, pair<int, int>& position)
{
    auto board = game->getBoard();
    Pieces* attackedPiece = board->getPiece(position);
    auto currentplayer = game->currentPlayer;
    auto opponent = game->getOpponent();

    if (attackedPiece == nullptr)
    {
        cerr << "No target found" << endl;
        return;
    }
    auto myvalue=mypiece->getValue();
    auto mytype=mypiece->type;
    auto othersvalue=attackedPiece->getValue();
    auto otherstype=attackedPiece->type;

    if (otherstype == PieceType::Bomb) {
        if (mytype == PieceType::Miner) {
            cout << "Good job ! The bomb is no more.\n" << endl;
            game->addCaptured(attackedPiece, currentplayer);
            mypiece->setCoord(position);
            game->removePiece(attackedPiece, opponent);
            return;
        } else {
            cout << "Rest well ! The war is over for you.\n" << endl;
            game->addCaptured(mypiece, opponent);
            board->removeFromBoard(mypiece);
            game->removePiece(mypiece, currentplayer);
            return;
        }
    }

    if (otherstype == PieceType::Marshal && mytype == PieceType::Spy) {
        cout << "Well done sir ! Their leader is gone.\n" << endl;
        game->addCaptured(attackedPiece, currentplayer);
        mypiece->setCoord(position);
        game->removePiece(attackedPiece, opponent);
        return;
    }


    if (myvalue > othersvalue) {
        cout << "The enemy is down ! It was a " << otherstype << ".\n" << endl;
        game->addCaptured(attackedPiece, currentplayer);
        mypiece->setCoord(position);
        game->removePiece(attackedPiece, opponent);
    }
    else if (myvalue < othersvalue) {
        cout << "The enemy is too strong ! It was a " << otherstype << ".\n" << endl;
        board->removeFromBoard(mypiece);
        game->removePiece(mypiece, currentplayer);
        game->addCaptured(mypiece, opponent);
    }
    else {
        cout << "It's a tie ! It was a " << otherstype << " too.\n" << endl;
        game->addCaptured(attackedPiece, currentplayer);
        game->addCaptured(mypiece, opponent);
        board->removeFromBoard(mypiece);
        board->removeFromBoard(attackedPiece);
        game->removePiece(mypiece, currentplayer);
        game->removePiece(attackedPiece, opponent);

    }
}

bool Engine::isValidMove(Pieces* piece, pair<int, int> to)
{
    auto possiblePositions = game->PossiblePositions(piece);
    for (size_t i = 0; i < possiblePositions.size(); i++)
    {
        if (possiblePositions[i] == to)
        {
            return true;
        }
    }
    return false;
}
