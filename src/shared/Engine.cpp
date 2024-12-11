//
// Created by plt on 11/12/24.
//
#include <iostream>

#include "engine.h"

using namespace engine;
using namespace state;
using namespace std;

void Engine::endTurn () {
    game->switchTurn();
}

bool Engine::handleCmd (pair<int,int> from, pair<int,int> to) {
    auto mypiece=Board::getPiece(from);
    auto currentplayer=game->currentPlayer;
    if (!currentplayer->belongTo(mypiece)) {
        return false;
    }
    if (!isValidMove(mypiece,to)) {
        return false;
    }
    auto targetpiece=Board::getPiece(to);
    if (targetpiece==nullptr) {
        mypiece->setPosition(to);
        endTurn();
        return true;
    }
    attack(mypiece,to);
    endTurn();
    return true;
}

Player * Engine::getOpponent () {
    if (game->currentPlayer==game->getPlayer1()) {
        return game->getPlayer2();
    }
    return game->getPlayer1();
}


void Engine::attack(Pieces *mypiece,pair<int, int> &position) {
    Board *board = Board::getInstance();
    Pieces *attackedPiece = board->getPiece(position);
    auto currentplayer = game->currentPlayer;
    auto opponent=getOpponent();

    if (attackedPiece == nullptr) {
        cerr << "No target found" << endl;
        return;
    }

    auto myname=mypiece->getName();
    auto othersName=attackedPiece->getName();

    if ( othersName== "Bomb") {
        if (myname == "Miner") {

            cout << "Good job ! The bomb is no more.\n" << endl;
            currentplayer->addCaptured(attackedPiece);
            mypiece->setPosition(position);
            opponent->removePiece(attackedPiece);
            return;
        }

        cout << "Rest well ! The war is over for you.\n" << endl;
        currentplayer->addCaptured(attackedPiece);
        board->removeFromBoard(mypiece);
        currentplayer->removePiece(mypiece);
        opponent->addPiece(mypiece);
        opponent->removePiece(attackedPiece);
        return;
    }

    if (othersName == "Marshal" && myname == "Spy") {
        cout << "Well done sir ! Their leader is gone.\n" << endl;
        currentplayer->addCaptured(attackedPiece);
        mypiece->setPosition(position);
        opponent->removePiece(attackedPiece);
        return;
    }

    if (mypiece->getValue() > attackedPiece->getValue()) {
        cout << "The enemy is down ! It was a " << othersName << ".\n" << endl;
        currentplayer->addCaptured(attackedPiece);
        mypiece->setPosition(position);
        opponent->removePiece(attackedPiece);

    } else if (mypiece->getValue() < attackedPiece->getValue()) {
        cout << "The enemy is too strong ! It was a " << othersName << ".\n" << endl;
        board->removeFromBoard(mypiece);
        currentplayer->removePiece(mypiece);
        opponent->addPiece(mypiece);

    } else {
        cout << "It's a tie ! It was a " << othersName << " too.\n" << endl;
        currentplayer->addCaptured(attackedPiece);
        board->removeFromBoard(mypiece);
        currentplayer->removePiece(mypiece);
        opponent->addPiece(mypiece);
        opponent->removePiece(attackedPiece);
    }
}

bool Engine::isValidMove (Pieces *piece, pair<int,int> to) {
    auto possiblePositions=piece->canMove(piece);
    for (int i=0; i<possiblePositions.size();i++) {
        if (possiblePositions[i]==to) {
            return true;
        }
    }
    return false;
}