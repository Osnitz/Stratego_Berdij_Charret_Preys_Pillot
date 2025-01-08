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

int ai::HeuristicAI::heuristicCalculator(Pieces* piece, const std::pair<int, int>& position, Game* game) {
    int weight = 0;
    auto currentPlayer = game->getCurrentPlayer();
    auto opponentPlayer = game->getOpponent();
    int totalMyPieces = currentPlayer->getMyPieces().size();

    if (piece->getType() == Scout) {
        weight += 50;
    }

    for (const auto& enemy : opponentPlayer->getKnown()) {
        int distance = abs(position.first - enemy->getPosition().first) +
                       abs(position.second - enemy->getPosition().second);

        if (distance == 2 && piece->getType() == Miner) {
            weight += 150;
        }

        if (distance == 1) {
            if (piece->getValue() > enemy->getValue()) {
                weight += 150;
            } else if (piece->getValue() < enemy->getValue()) {
                weight -= 100;
            }
        }

        if (distance == 0) {
            if ((piece->getType() == Spy) && (enemy->getType() == Marshal)) {
                weight += 500;
            } else if (piece->getValue() > enemy->getValue()) {
                weight += 300;
            } else if (piece->getValue() < enemy->getValue()) {
                weight -= 200;
            }
        }
    }

    if (opponentPlayer->getKnown().empty()) {
        if (piece->getType() != Miner) {
            weight += 20;
            if (position.second > piece->getPosition().second) {
                weight += 30;
            }
        }
    }

    for (const auto& enemy : opponentPlayer->getKnown()) {
        if (piece->getValue() > enemy->getValue()) {
            int distance = abs(position.first - enemy->getPosition().first) +
                           abs(position.second - enemy->getPosition().second);
            if (distance < 5) {
                weight += 50;
            }
        }
    }

    for (const auto& enemy : opponentPlayer->getKnown()) {
        if (immobileCounter[enemy] >= 10) { // Seuil d'immobilité
            int distance = abs(position.first - enemy->getPosition().first) +
                           abs(position.second - enemy->getPosition().second);

            if (piece->getType() == Miner) {
                weight += 300 / (distance + 1);
            } else {
                weight += 50 / (distance + 1);
            }
        }
    }

    for (const auto& ally : currentPlayer->getMyPieces()) {
        if (ally->getPosition() != position) {
            int distance = abs(position.first - ally->getPosition().first) +
                           abs(position.second - ally->getPosition().second);
            if (distance <= 2) {
                weight += 10;
            } else if (distance > 5) {
                weight -= 10;
            }
        }
    }

    /*if (totalMyPieces > 25) { // Début de partie
        if (piece->getType() != Miner && piece->getType() != Flag) {
            weight += 20; // Favoriser les pièces non essentielles pour explorer
        }
    } else if (totalMyPieces > 10) { // Milieu de partie
        weight += 10; // Bonus générique pour toutes les actions stratégiques
    } else { // Fin de partie
        if (piece->getType() == Miner) {
            weight += 50; // Protection des Miners
        } else if (piece->getType() == Flag) {
            weight -= 100; // Décourager tout mouvement de la Flag
        }
    }*/

    weight += 10;

    return weight;
}


/*vector<std::pair<std::pair<int, int>, int>> ai::HeuristicAI::weightedRanking(Pieces* piece, Game* game) {
    auto possibleMoves = getPossibleMoves(piece, game);
    std::vector<std::pair<std::pair<int, int>, int>> weightedMoves;

    for (const auto& position : possibleMoves) {
        int weight = heuristicCalculator(*piece, position, game);
        weightedMoves.emplace_back(position, weight);
    }

    std::sort(weightedMoves.begin(), weightedMoves.end(),
              [](pair<pair<int,int>,int>& a, pair<pair<int,int>,int>& b) { return a.second > b.second; });

    return weightedMoves;
}*/

vector<Pieces*> ai::HeuristicAI::getPlayablePieces(Game* game) {
    std::vector<Pieces*> playablePieces;
    auto currentPlayer=game->getCurrentPlayer();
    for (auto& piece : currentPlayer->getMyPieces()) {
        auto possibleMoves = getPossibleMoves(piece, game);
        if (!possibleMoves.empty()) {
            playablePieces.push_back(piece);
        }
    }
    return playablePieces;
}

void ai::HeuristicAI::immobileCounting(state::Game* game) {
    auto currentBoard = game->getBoard()->grid;
    if (boardSnapshot.empty()) {
        boardSnapshot = currentBoard;
        return;
    }

    for (int row = 0; row < currentBoard.size(); ++row) {
        for (int col = 0; col < currentBoard[row].size(); ++col) {
            auto currentPiece = currentBoard[row][col];
            auto previousPiece = boardSnapshot[row][col];

            if (currentPiece != nullptr && currentPiece == previousPiece) {
                immobileCounter[currentPiece]++;
            } else if (currentPiece != nullptr) {
                immobileCounter[currentPiece] = 0;
            }
        }
    }

    boardSnapshot = currentBoard;
}


std::vector<int> ai::HeuristicAI::calculateMove(Game* game) {
    std::vector<int> positions;
    int bestWeight = -1;
    Pieces* bestPiece = nullptr;
    std::pair<int, int> bestPosition = {-1, -1};

    auto playablePieces = getPlayablePieces(game);

    for (auto piece : playablePieces) {

        auto possibleMoves=getPossibleMoves(piece,game);

        for (auto& move : possibleMoves) {
            int weight=heuristicCalculator(piece,move,game);
            if (weight > bestWeight) {
                bestWeight = weight;
                bestPiece = piece;
                bestPosition = move;
            }
        }
    }

    if (bestPiece!=nullptr){
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

