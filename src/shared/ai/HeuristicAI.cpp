//
// Created by plt on 18/12/24.
//

#include "ai.h"
#include "HeuristicAI.h"
#include <algorithm>
#include <iostream>
#include <state/Game.h>
#include <state/PieceType.h>
#include <state/Pieces.h>
#include <state/Player.h>

int ai::HeuristicAI::heuristicCalculator(state::Pieces& piece, const std::pair<int, int>& position) {
    int weight = 0;
    state::Player currentPlayer;

    if (piece.getType() == state::PieceType::Scout) {
        weight += 50;
    }

    if (piece.getType() == state::PieceType::Miner || piece.getType() == state::PieceType::Flag) {
        for (const auto& enemy : currentPlayer.knownPieces) {
            int distance = abs(position.first - enemy->state::Pieces::getPosition().first) + abs(position.second - enemy->getPosition().second);
            if (distance <= 2) {
                weight += 100;
            }
        }
    }

    for (const auto& enemy : currentPlayer.knownPieces) {
        if (piece.getValue() > enemy->getValue()) {
            int distance = abs(position.first - enemy->state::Pieces::getPosition().first) + abs(position.second - enemy->state::Pieces::getPosition().second);
            if (distance == 1) {
                weight += 200;
            }
        }
    }

    weight += 10;

    return weight;
}

std::vector<std::pair<std::pair<int, int>, int>> ai::HeuristicAI::calculateMove(state::Pieces* piece) {
    state::Game gameInstance;
    auto possibleMoves = gameInstance.possiblePositions(piece);
    std::vector<std::pair<std::pair<int, int>, int>> weightedMoves;

    for (const auto& pos : possibleMoves) {
        int weight = heuristicCalculator(*piece, pos);
        weightedMoves.push_back({pos, weight});
    }

    std::sort(weightedMoves.begin(), weightedMoves.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    return weightedMoves;
}

std::pair<state::Pieces*, std::pair<int, int>> ai::HeuristicAI::bestMove() {
    int bestWeight = -1;
    state::Pieces* bestPiece = nullptr;
    std::pair<int, int> bestPosition = {-1, -1};

    auto playablePieces = getPlayablePieces();

    for (auto& piece : playablePieces) {
        auto moves = calculateMove(&piece);

        for (const auto& move : moves) {
            if (move.second > bestWeight) {
                bestWeight = move.second;
                bestPiece = &piece;
                bestPosition = move.first;
            }
        }
    }

    if (bestPiece) {
        return {bestPiece, bestPosition};
    }

    throw std::runtime_error("No valid moves found");
}
