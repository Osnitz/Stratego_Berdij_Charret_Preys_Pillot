//
// Created by matthieu on 16/12/24.
//
#include <iostream>
#include "RandomAI.h"

#include <random>

#include "state.h"
#include "engine.h"

using namespace ai;
using namespace state;
RandomAI::RandomAI()
{
}


std::vector<int> RandomAI::calculateMove(state::Game* game)
{
    std::vector<int> positions;
    int startX, startY, endX, endY;


    Player* aiPlayer = game->getCurrentPlayer();
    std::vector<Pieces*> aiPieces = aiPlayer->getMyPieces();
    if (aiPieces.empty())
    {
        std::cerr << "No pieces available for AI to move." << std::endl;
        return positions;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    // Filter the pieces with possible moves
    std::vector<Pieces*> movablePieces;
    for (auto* piece : aiPieces)
    {
        if (!game->possiblePositions(piece).empty())
        {
            movablePieces.push_back(piece);
        }
    }

    if (movablePieces.empty())
    {
        std::cerr << "No valid moves available for AI." << std::endl;
        return positions;
    }

    // Select a piece at random from those that can move
    std::uniform_int_distribution<> distr(0, movablePieces.size() - 1);
    Pieces* pieceToMove = movablePieces[distr(gen)];
    startX = pieceToMove->getPosition().first;
    startY = pieceToMove->getPosition().second;
    // Get the possible positions for this piece
    auto possiblePositions = game->possiblePositions(pieceToMove);
    std::uniform_int_distribution<> distr2(0, possiblePositions.size() - 1);
    auto destination = possiblePositions[distr2(gen)];

    endX = destination.first;
    endY = destination.second;

    positions.push_back(startX);
    positions.push_back(startY);
    positions.push_back(endX);
    positions.push_back(endY);

    return positions;
}

RandomAI::~RandomAI()
{
}