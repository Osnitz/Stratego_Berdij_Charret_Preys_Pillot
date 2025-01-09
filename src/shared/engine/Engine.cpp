//
// Created by plt on 11/12/24.
//
#include <iostream>
#include <state/Pieces.h>
#include <state/PieceType.h>
#include <state/Player.h>
#include <state.h>
#include <state/Game.h>

#include "../engine.h"

using namespace engine;
using namespace state;
using namespace std;

Engine::Engine(Game* game): game(game)
{
}


void Engine::endTurn()
{
    game->switchTurn();
}

bool Engine::handleCmdPlacement(string filePath)
{
    game->loadConfig(filePath);
    endTurn();
    return true;
}

bool Engine::handleCmdMove(pair<int, int> from, pair<int, int> to)
{
    auto board=game->getBoard();
    auto mypiece = board->getPiece(from);
    if (!game->belongTo(mypiece))
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
    if (game->isEmpty(targetpiece))
    {
        game->setPieceOnBoard(mypiece,to.first,to.second);
        endTurn();
        return true;
    }
    attack(mypiece, to);
    endTurn();
    return true;
}

void Engine::attack(Pieces* mypiece, pair<int, int> position)
{
    auto board = game->getBoard();
    Pieces* attackedPiece = board->getPiece(position);
    auto currentplayer = game->getCurrentPlayer();
    auto opponent = game->getOpponent();


    if (attackedPiece == nullptr)
    {
        cerr << "No target found" << endl;
        return;
    }
    auto myvalue=mypiece->getValue();
    auto mytype=mypiece->getType();
    auto attackedvalue=attackedPiece->getValue();
    auto attackedtype=attackedPiece->getType();


    if (attackedtype == PieceType::Bomb) {
        if (mytype == PieceType::Miner) {
            cout << "Good job ! The bomb is no more.\n" << endl;
            game->addCaptured(attackedPiece, currentplayer);
            game->removeFromBoard(attackedPiece);
            game->removePiece(attackedPiece, opponent);
            game->setPieceOnBoard(mypiece,position.first,position.second);
            return;
        } else {
            cout << "Rest well ! The war is over for you.\n" << endl;
            game->addCaptured(mypiece, opponent);
            game->removeFromBoard(mypiece);
            game->removePiece(mypiece, currentplayer);
            game->addCaptured(attackedPiece, currentplayer);
            game->removeFromBoard(attackedPiece);
            game->removePiece(attackedPiece, opponent);
            return;
        }
    }


    if (attackedtype == PieceType::Marshal && mytype == PieceType::Spy) {
        cout << "Well done sir ! Their leader is gone.\n" << endl;
        game->addCaptured(attackedPiece, currentplayer);
        game->removeFromBoard(attackedPiece);
        game->removePiece(attackedPiece, opponent);
        game->setPieceOnBoard(mypiece,position.first,position.second);
        game->addKnown(mypiece,opponent);
        return;
    }




    if (myvalue > attackedvalue) {
        cout << "The enemy is down ! It was a " << attackedtype << ".\n" << endl;
        game->addCaptured(attackedPiece, currentplayer);
        game->removeFromBoard(attackedPiece);
        game->removePiece(attackedPiece, opponent);
        game->setPieceOnBoard(mypiece,position.first,position.second);
        game->addKnown(mypiece,opponent);
    }
    else if (myvalue < attackedvalue) {
        cout << "The enemy is too strong ! It was a " << attackedtype << ".\n" << endl;
        game->removeFromBoard(mypiece);
        game->removePiece(mypiece, currentplayer);
        game->addCaptured(mypiece, opponent);
        game->addKnown(attackedPiece,currentplayer);
    }
    else {
        cout << "It's a tie ! It was a " << attackedtype << " too.\n" << endl;
        game->addCaptured(attackedPiece, currentplayer);
        game->addCaptured(mypiece, opponent);
        game->removeFromBoard(mypiece);
        game->removeFromBoard(attackedPiece);
        game->removePiece(mypiece, currentplayer);
        game->removePiece(attackedPiece, opponent);


    }
}


bool Engine::isValidMove(Pieces* piece, pair<int, int> to)
{
    auto possiblePositions = game->possiblePositions(piece);
    for (size_t i = 0; i < possiblePositions.size(); i++)
    {
        if (possiblePositions[i] == to)
        {
            return true;
        }
    }
    return false;
}

Game * Engine::getGame() {
    return game;
}

WinCondition Engine::checkWin()
{
    if (game->isFlagCaptured())
    {
        return WinCondition::FlagCaptured;
    }

    if (!game->hasValidMoves())
    {
        return WinCondition::NoValidMoves;
    }

    return WinCondition::None;
}