//
// Created by plt on 18/12/24.
//

#include "ai.h"
#include "HeuristicAI.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <state/Game.h>
#include <state/PieceType.h>
#include <state/Pieces.h>
#include <state/Player.h>

using namespace state;
using namespace std;

ai::HeuristicAI::HeuristicAI() {
    unordered_map<Pieces*, vector<pair<int, int>>> possibleMovesCache;
}

ai::HeuristicAI::~HeuristicAI() = default;

void ai::HeuristicAI::clearCache() {
    possibleMovesCache.clear();
}

vector<pair<int, int>> ai::HeuristicAI::getPossibleMoves(Pieces* piece, Game* game) {
    if (possibleMovesCache.find(piece) == possibleMovesCache.end()) {
        possibleMovesCache[piece] = game->possiblePositions(piece);
    }
    return possibleMovesCache[piece];
}

int ai::HeuristicAI::heuristicCalculator(state::Pieces& piece, const std::pair<int, int>& position, state::Game* game) {
    int weight = 0;
    auto currentPlayer=game->getCurrentPlayer();

    if (piece.getType() == state::PieceType::Scout) {
        weight += 50;
    }

    for (const auto& enemy : currentPlayer->knownPieces) {
        int distance = abs(position.first - enemy->state::Pieces::getPosition().first) +
                                   abs(position.second - enemy->state::Pieces::getPosition().second);
        if (distance <= 2) {
             if (piece.getType() == state::PieceType::Miner) {
                weight += 100;
            }
        }

        if (distance == 1) {
            if ((piece.getType()== state::PieceType::Spy) && (enemy->getType() == state::PieceType::Marshal)) {
                weight += 500;
            }
            if (piece.getValue() > enemy->getValue()) {
                weight += 200;
            }
            if (piece.getValue() < enemy->getValue()) {
                weight -= 100;
            }
        }
    }

    weight += 10;

    return weight;
}

vector<std::pair<std::pair<int, int>, int>> ai::HeuristicAI::weightedRanking(Pieces* piece, Game* game) {
    auto possibleMoves = getPossibleMoves(piece, game);
    std::vector<std::pair<std::pair<int, int>, int>> weightedMoves;

    for (const auto& position : possibleMoves) {
        int weight = heuristicCalculator(*piece, position, game);
        weightedMoves.push_back({position, weight});
    }

    std::sort(weightedMoves.begin(), weightedMoves.end(),
              [](pair<pair<int,int>,int>& a, pair<pair<int,int>,int>& b) { return a.second > b.second; });

    return weightedMoves;
}

vector<Pieces*> ai::HeuristicAI::getPlayablePieces(state::Game* game) {
    std::vector<state::Pieces*> playablePieces;
    auto currentPlayer=game->getCurrentPlayer();
    for (auto& piece : currentPlayer->getMyPieces()) {
        auto possibleMoves = getPossibleMoves(piece, game);
        if (!possibleMoves.empty()) {
            playablePieces.push_back(piece);
        }
    }
    return playablePieces;
}

std::vector<int> ai::HeuristicAI::calculateMove(Game* game) {
    std::vector<int> positions;
    int bestWeight = -1;
    Pieces* bestPiece = nullptr;
    std::pair<int, int> bestPosition = {-1, -1};

    auto playablePieces = getPlayablePieces(game);

    for (const auto& piece : playablePieces) {
        auto moves = weightedRanking(piece, game);

        for (const auto& move : moves) {
            if (move.second > bestWeight) {
                bestWeight = move.second;
                bestPiece = piece;
                bestPosition = move.first;
            }
        }
    }

    if (bestPiece){
        auto currentPosition = bestPiece->getPosition();
        auto currentX = currentPosition.first;
        auto currentY = currentPosition.second;
        auto newX = bestPosition.first;
        auto newY = bestPosition.second;

        positions.push_back(currentX);
        positions.push_back(currentY);
        positions.push_back(newX);
        positions.push_back(newY);

        clearCache();
        return positions;
    }

    throw std::runtime_error("No valid moves found");
}

