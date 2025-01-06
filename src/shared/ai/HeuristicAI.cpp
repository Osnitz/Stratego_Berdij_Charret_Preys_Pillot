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

void ai::HeuristicAI::clearCache() {
    possibleMovesCache.clear();
}

std::vector<std::pair<int, int>> ai::HeuristicAI::getPossibleMoves(state::Pieces* piece, state::Game* game) {
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

    else if (piece.getType() == state::PieceType::Miner || piece.getType() == state::PieceType::Flag) {
        for (const auto& enemy : currentPlayer->knownPieces) {
            int distance = abs(position.first - enemy->state::Pieces::getPosition().first) +
                           abs(position.second - enemy->getPosition().second);
            if (distance <= 2) {
                weight += 100;
            }
        }
    }

    for (const auto& enemy : currentPlayer->knownPieces) {
        if (piece.getValue() > enemy->getValue()) {
            int distance = abs(position.first - enemy->state::Pieces::getPosition().first) +
                           abs(position.second - enemy->state::Pieces::getPosition().second);
            if (distance == 1) {
                weight += 200;
            }
        }
    }

    weight += 10;

    return weight;
}

std::vector<std::pair<std::pair<int, int>, int>> ai::HeuristicAI::weightedRanking(state::Pieces* piece, state::Game* game) {
    auto possibleMoves = getPossibleMoves(piece, game);
    std::vector<std::pair<std::pair<int, int>, int>> weightedMoves;

    for (const auto& pos : possibleMoves) {
        int weight = heuristicCalculator(*piece, pos, game);
        weightedMoves.push_back({pos, weight});
    }

    std::sort(weightedMoves.begin(), weightedMoves.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    return weightedMoves;
}

std::vector<state::Pieces*> ai::HeuristicAI::getPlayablePieces(state::Game* game) {
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

std::vector<int> ai::HeuristicAI::calculateMove(state::Game* game) {
    std::vector<int> positions;
    int bestWeight = -1;
    state::Pieces* bestPiece = nullptr;
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
