//
// Created by plt on 11/12/24.
//
#include <iostream>

#include "../engine.h"

using namespace engine;
using namespace state;
using namespace std;

Engine::Engine(Game* game, std::vector<Player*> players)
    : game(game), players(players)
{
}


void Engine::startGame()
{
    currentPlayerIndex = 0; // Commence avec le premier joueur
    game->switchTurn();
    std::cout << "Game started. Entering Placement Phase." << std::endl;
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
    if (targetpiece == nullptr)
    {
        mypiece->setPosition(to);
        endTurn();
        return true;
    }
    attack(mypiece, to);
    endTurn();
    return true;
}

Player* Engine::getOpponent()
{
    if (game->currentPlayer == game->getPlayer1())
    {
        return game->getPlayer2();
    }
    return game->getPlayer1();
}


void Engine::attack(const pair<int, int> &position) {
	Board *board = Board::getInstance();
    Pieces *attackedPiece = board->getPiece(position);
    auto player = game->getCurrentPlayer();
    auto opponent = game->getOpponent();

    if (attackedPiece == nullptr) {
        cerr << "No target found" << endl;
        return;
    }

    if (attackedPiece->type == PieceType::Bomb) {
        if (type == PieceType::Miner) {
            cout << "Good job ! The bomb is no more.\n" << endl;
            game->addCaptured(attackedPiece, player);
            setPosition(position);
            game->removePiece(attackedPiece, opponent);
            return;
        } else {
            cout << "Rest well ! The war is over for you.\n" << endl;
            game->addCaptured(this, opponent);
            board->removeFromBoard(this);
            game->removePiece(this, player);
            return;
        }
    }

    if (attackedPiece->type == PieceType::Marshal && type == PieceType::Spy) {
        cout << "Well done sir ! Their leader is gone.\n" << endl;
        game->addCaptured(attackedPiece, player);
        setPosition(position);
        game->removePiece(attackedPiece, opponent);
        return;
    }


    if (value > attackedPiece->value) {
        cout << "The enemy is down ! It was a " << attackedPiece->type << ".\n" << endl;
        game->addCaptured(attackedPiece, player);
        setPosition(position);
        game->removePiece(attackedPiece, opponent);
    }
    else if (value < attackedPiece->value) {
        cout << "The enemy is too strong ! It was a " << attackedPiece->type << ".\n" << endl;
        board->removeFromBoard(this);
        game->removePiece(this, player);
        game->addCaptured(this, opponent);
    }
    else {
        cout << "It's a tie ! It was a " << attackedPiece->type << " too.\n" << endl;
        game->addCaptured(attackedPiece, player);
        game->addCaptured(this, opponent);
        board->removeFromBoard(this);
        board->removeFromBoard(attackedPiece);
        game->removePiece(this, player);
        game->removePiece(attackedPiece, opponent);
    }
}

bool Engine::isValidMove(Pieces* piece, pair<int, int> to)
{
    auto possiblePositions = piece->canMove(piece);
    for (size_t i = 0; i < possiblePositions.size(); i++)
    {
        if (possiblePositions[i] == to)
        {
            return true;
        }
    }
    return false;
}
