//
// Created by matthieu on 14/10/24.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "../state.h"
#include "../engine.h"
#include "random"
using namespace state;
using namespace engine;
using namespace std;

Game::Game()
{
    board = new Board();
    Player1 = new Player(0);
    Player2 = new Player(1);
    currentPlayer = nullptr;

    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(1, 2); // Define the range
    int randomPlayer = distr(gen); // Generate the random number
    if (randomPlayer == 1) {
        currentPlayer = Player1;
    } else {
        currentPlayer = Player2;
    }
}

Game::~Game()
{
    delete board;
    delete Player1;
    delete Player2;
    //delete currentState;
}


void Game::switchTurn()
{
    if (currentPlayer == Player1)
    {
        currentPlayer = Player2;
    }
    else
    {
        currentPlayer = Player1;
    }
}

Player* Game::getCurrentPlayer()
{
    if (currentPlayer == nullptr)
    {
        throw std::invalid_argument("No current player");
    }
    return currentPlayer;
}

Player* Game::getPlayer1()
{
    return Player1;
}

Player* Game::getPlayer2()
{
    return Player2;
}

void Game::setCurrentPlayer(Player* player)
{
    currentPlayer = player;
}

void Game::setPieceOnBoard(Pieces* piece) {
    pair<int, int> position = piece->getPosition();
    auto grid=board->getGrid();
    *grid[position.first][position.second] = piece;
}

void Game::removeFromBoard(Pieces* piece) {
    pair<int, int> position = piece->getPosition();
    auto grid=board->getGrid();
    grid[position.first][position.second] = nullptr;
}

vector<pair<int, int>> Game::PossiblePositions(Pieces* pieceToMove) {
    vector<pair<int, int>> possiblePositions;

    if (pieceToMove == nullptr) {
        return possiblePositions;
    }

    int x = pieceToMove->getPosition().first;
    int y = pieceToMove->getPosition().second;
    int range = pieceToMove->getRange();
    pair<int,int> posToCheck;

    for(int j=1; j<=4;j++) {
        for (int i = 1; i <= range; ++i) {
            switch (j) {
                case 1: posToCheck = {x, y - i};
                break;
                case 2: posToCheck = {x, y + i};
                break;
                case 3: posToCheck = {x-i, y };
                break;
                case 4: posToCheck = {x+i, y};
                break;
            }
            Pieces *targetPiece=board->getPiece(posToCheck);

            if (pieceToMove->LimitBoard(posToCheck,true)) {
                if (IsAlly(targetPiece)){
                    break;
                }
                possiblePositions.push_back(posToCheck);
                if (IsEnemy(targetPiece)) {
                    break;
                }
            }
        }
    }
    return possiblePositions;
}

bool Game::belongTo(Pieces* piece) {
    auto myPieces =currentPlayer->getMyPieces();
    for (size_t i = 0; i < myPieces.size(); i++) {
        if (myPieces[i] == piece) {
            return true;
        }
    }
    return false;
}

void Game::addPiece(Pieces* piece, Player * player) {
    auto myPieces=player->getMyPieces();
    myPieces.push_back(piece);
}

void Game::loadConfig(const string& fileName){
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "File " << fileName << " not found " << endl;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        cout << line << endl;
        stringstream ss(line);
        string cell;
        vector<string> dataline;

        while (getline(ss, cell, ',')) {
            cell.erase(remove(cell.begin(), cell.end(), '"'), cell.end());
            dataline.push_back(cell);
        }

        auto type = static_cast<PieceType>(stoi(dataline.at(0)));
        int value = stoi(dataline.at(1));
        int x = stoi(dataline.at(2));
        int y = stoi(dataline.at(3));

        if (currentPlayer == Player1) {
            auto * piece = new Pieces(value, type, x, y, true);
            currentPlayer->getMyPieces().push_back(piece);
            board->setPieceOnBoard(piece);
        }
        else {
            auto * piece = new Pieces(value, type, 9 - x, y, false);
            currentPlayer->getMyPieces().push_back(piece);
            board->setPieceOnBoard(piece);
        }
    }
    board->displayBoard(*currentPlayer);
}

bool Game::LimitBoard(const pair<int, int>& &position, bool silent){
    int NewX = position.first;
    int NewY = position.second;
    if ((NewX < 0) || (NewY < 0) || (NewX > 9) || (NewY > 9)) {
        if (!silent)
        {
            cerr << "Out of bounds" << endl;
        }
        return false;
    }
    if ((NewX == 4) || (NewX == 5)) {
        if ((NewY == 2) || (NewY == 3) || (NewY == 6) || (NewY == 7)) {
            if (!silent)
            {
                cerr << "You can't cross lakes !" << endl;
            }
            return false;
        }
    }
    return true;
}

bool Game::IsEmpty (const Pieces * targetPiece) {
    if (targetPiece == nullptr) {
        return true;
    }
    return false;
}

bool Game::IsAlly(const Pieces *targetPiece) {
    if(currentPlayer==targetPiece->Playerid) {
        return true;
    }
    return false;
}

bool Game::IsEnemy(const Pieces *targetPiece) {
    if(currentPlayer!=targetPiece->Playerid) {
        return true;
    }
    return false;
}