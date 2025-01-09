//
// Created by matthieu on 14/10/24.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <unordered_map>


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

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_int_distribution<> distr(1, 2); // Define the range
    int randomPlayer = distr(gen); // Generate the random number
    if (randomPlayer == 1)
    {
        currentPlayer = Player1;
    }
    else
    {
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

void Game::addCaptured(Pieces* piece, Player* player)
{
    auto& capturedPieces = player->getCaptured();
    auto size = capturedPieces.size();
    int value = piece->getValue();
    for (std::size_t i = 0; i < size; i++)
    {
        int myvalue = capturedPieces[i]->getValue();
        if (value <= myvalue)
        {
            capturedPieces.insert(capturedPieces.begin() + i, piece);
            return;
        }
    }
    capturedPieces.push_back(piece);
}

void Game::removePiece(Pieces* piece, Player* player)
{
    auto& myPieces = player->getMyPieces();
    if (myPieces.empty())
    {
        cerr << "No piece left" << endl;
        return;
    }
    for (size_t i = 0; i < myPieces.size(); i++)
    {
        if (myPieces[i] == piece)
        {
            myPieces.erase(myPieces.begin() + i);
            return;
        }
    }
    cerr << "Can't remove this piece : it doesn't exist!" << endl;
}

Player* Game::getOpponent()
{
    if (currentPlayer == Player1)
    {
        return Player2;
    }
    return Player1;
}

bool Game::belongTo(Pieces* piece, Player* player)
{
    if (player == nullptr)
    {
        player = currentPlayer;
    }
    auto myPieces = player->getMyPieces();
    for (size_t i = 0; i < myPieces.size(); i++)
    {
        if (myPieces[i] == piece)
        {
            return true;
        }
    }
    return false;
}

void Game::addPiece(Pieces* piece, Player* player)
{
    player->getMyPieces().push_back(piece);
}

void Game::loadConfig(string& fileName)
{
    ifstream file(fileName);

    if (!file.is_open())
    {
        cerr << "File " << fileName << " not found " << endl;
    }
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        //cout << line << endl;
        stringstream ss(line);
        string cell;
        vector<string> dataline;

        while (getline(ss, cell, ','))
        {
            cell.erase(remove(cell.begin(), cell.end(), '"'), cell.end());
            dataline.push_back(cell);
        }

        std::unordered_map<std::string, state::PieceType> pieceTypeMap = {
            {"Flag", state::PieceType::Flag},
            {"Bomb", state::PieceType::Bomb},
            {"Spy", state::PieceType::Spy},
            {"Scout", state::PieceType::Scout},
            {"Miner", state::PieceType::Miner},
            {"Sergeant", state::PieceType::Sergeant},
            {"Lieutenant", state::PieceType::Lieutenant},
            {"Captain", state::PieceType::Captain},
            {"Major", state::PieceType::Major},
            {"Colonel", state::PieceType::Colonel},
            {"General", state::PieceType::General},
            {"Marshal", state::PieceType::Marshal}
        };

        PieceType type;
        auto it = pieceTypeMap.find(dataline.at(0));
        if (it != pieceTypeMap.end())
        {
            type = it->second;
        }
        else
        {
            throw std::invalid_argument("Invalid PieceType: " + dataline.at(0));
        }


        int value = stoi(dataline.at(1));
        int x = stoi(dataline.at(2));
        int y = stoi(dataline.at(3));

        if (currentPlayer == Player1)
        {
            auto* piece = new Pieces(value, type, x, y, Player1);
            currentPlayer->getMyPieces().push_back(piece);
            setPieceOnBoard(piece, x, y);
        }
        else
        {
            auto* piece = new Pieces(value, type, 9 - x, y, Player2);
            currentPlayer->getMyPieces().push_back(piece);
            setPieceOnBoard(piece, 9 - x, y);
        }
    }
    displayBoard(*currentPlayer);
}

bool Game::limitBoard(pair<int, int>& position, bool silent)
{
    int NewX = position.first;
    int NewY = position.second;
    if ((NewX < 0) || (NewY < 0) || (NewX > 9) || (NewY > 9))
    {
        if (!silent)
        {
            cerr << "Out of bounds" << endl;
        }
        return false;
    }
    if ((NewX == 4) || (NewX == 5))
    {
        if ((NewY == 2) || (NewY == 3) || (NewY == 6) || (NewY == 7))
        {
            if (!silent)
            {
                cerr << "You can't cross lakes !" << endl;
            }
            return false;
        }
    }
    return true;
}

bool Game::isAlly(Pieces* targetPiece)
{
    if (currentPlayer == targetPiece->getOwner())
    {
        return true;
    }
    return false;
}

bool Game::isEnemy(Pieces* targetPiece)
{
    if (currentPlayer != targetPiece->getOwner())
    {
        return true;
    }
    return false;
}

bool Game::isEmpty(Pieces* targetPiece)
{
    if (targetPiece == nullptr)
    {
        return true;
    }
    return false;
}

void Game::setPieceOnBoard(Pieces* piece, int newX, int newY)
{
    removeFromBoard(piece);
    piece->setCoord(newX, newY);
    auto& grid = *board->getGrid();
    grid[newX][newY] = piece;
}

void Game::removeFromBoard(Pieces* piece)
{
    pair<int, int> position = piece->getPosition();
    auto& grid = *board->getGrid();
    grid[position.first][position.second] = nullptr;
}

vector<pair<int, int>> Game::possiblePositions(Pieces* pieceToMove)
{
    vector<pair<int, int>> possiblePositions;

    if (pieceToMove == nullptr)
    {
        return possiblePositions;
    }

    int x = pieceToMove->getPosition().first;
    int y = pieceToMove->getPosition().second;
    int range = pieceToMove->getRange();
    pair<int, int> posToCheck;

    for (int j = 1; j <= 4; j++)
    {
        for (int i = 1; i <= range; ++i)
        {
            switch (j)
            {
                case 1: posToCheck = {x, y - i};
                break;
                case 2: posToCheck = {x, y + i};
                break;
                case 3: posToCheck = {x - i, y};
                break;
                case 4: posToCheck = {x + i, y};
                break;
            }
            if (!limitBoard(posToCheck, true))
            {
                break; // Si hors limites, arrêter cette direction
            }

            Pieces* targetPiece = board->getPiece(posToCheck);

            if (targetPiece == nullptr) //empty case
            {
                possiblePositions.push_back(posToCheck);
                continue;
            }

            if (isAlly(targetPiece)) //Occupied by an Ally
            {
                break;
            }

            possiblePositions.push_back(posToCheck); //Occupied by an Enemy
            break;
        }
    }
    return possiblePositions;
}

Board* Game::getBoard()
{
    return board;
}

void Game::displayBoard(Player& player)
{
    auto& grid = *board->getGrid();
    // Print the first line of column's number in the grid
    cout << "    "; // Initial alignment for the first row of the grid
    for (size_t col = 0; col < grid[0].size(); ++col)
    {
        cout << "   " << col << "   "; // Adjusting the spacing for column numbers
    }
    cout << endl;

    // Display the bounding line under column numbers
    std::cout << "   "; // Initial alignment for dashes
    for (std::size_t col = 0; col < grid[0].size(); ++col)
    {
        std::cout << "-------"; // Horizontal delineation
    }
    std::cout << "-" << std::endl; // End of the delineation to the right

    // Print the grid with the pieces
    for (size_t row = 0; row < grid.size(); ++row)
    {
        cout << row << "  "; // Print line's numbers inside the grid
        for (size_t col = 0; col < grid[row].size(); ++col)
        {
            if ((row == 4 && (col == 2 || col == 3 || col == 6 || col == 7)) || (row == 5 && (col == 2 || col == 3 ||
                col == 6 || col == 7)))
            {
                cout << "|  XX  "; // Lacs
            }
            else
            {
                Pieces* piece = grid[row][col];
                if (piece == nullptr)
                {
                    cout << "|      "; // Empty box
                }
                else if (belongTo(piece,&player))
                {
                    int value = piece->getValue();
                    if (value < 10)
                    {
                        cout << "|  0" << piece->getValue() << "  ";
                    }
                    else
                    {
                        cout << "|  " << piece->getValue() << "  ";
                    }
                }
                else
                {
                    cout << "|  ??  "; // Enemy's piece
                }
            }
        }
        cout << "|" << endl;

        // Display the bounding line under each line
        cout << "   "; // Alignment for dashes
        for (size_t col = 0; col < grid[0].size(); ++col)
        {
            cout << "-------"; // Horizontal delineation
        }
        cout << "-" << endl; // End of the delineation to the right
    }
}

void Game::setCurrentPlayer(Player* player)
{
    currentPlayer = player;
}

bool Game::isFlagCaptured()
{
    auto checkFlagCaptured = [](const std::vector<Pieces*>& capturedPieces)
    {
        if(capturedPieces.empty()){
            return false;
        }

        if(capturedPieces[0]->getType()==Flag){
            return true;
        }
        return false;
    };

    if (checkFlagCaptured(Player1->getCaptured()) || checkFlagCaptured(Player2->getCaptured()))
    {
        return true;
    }
    return false;
}

bool Game::hasValidMoves()
{
    auto& myPieces = currentPlayer->getMyPieces();
    if (!myPieces.empty())
    {
        for (const auto& piece : myPieces)
        {
            if (!possiblePositions(piece).empty())
            {
                return true;
            }
        }
    }
    return false;
}

void Game::clearBoard()
{
    std::vector<std::vector<Pieces*>>* grid = board->getGrid();
    if (!grid) {
        std::cerr << "Error: Grid is null." << std::endl;
        return;
    }

    for (std::vector<Pieces*>& row : *grid) // Déréférencement de grid avec *
    {
        for (auto& piece : row) // Utilisation de références pour modifier directement
        {
            if (piece != nullptr)
            {
                delete piece;
                piece = nullptr;
            }
        }
    }
}


Player* Game::getPlayerByID(int playerID)
{
    if (playerID == 0)
    {
        return Player1;
    }
    return Player2;
}

void Game::addKnown(Pieces* piece, Player* player)
{
    player->getKnown().push_back(piece);
}