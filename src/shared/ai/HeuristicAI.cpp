#include "ai.h"
#include "HeuristicAI.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <state/Board.h>
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
        auto moves = game->possiblePositions(piece);
        possibleMovesCache[piece] = moves;
    }
    return possibleMovesCache[piece];
}

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

int ai::HeuristicAI::heuristicCalculator(Pieces* piece, const std::pair<int, int>& position, Game* game) {
    int weight = 0;
    auto currentPlayer = game->getCurrentPlayer();
    auto opponentPlayer = game->getOpponent();
    Board* board = game->getBoard();
    std::pair<int, int> flagPosition = {-1, -1};

    for (const auto& pieces : currentPlayer->getMyPieces()) {
        if (pieces->getType() == Flag) {
            flagPosition = pieces->getPosition();
            break;
        }
    }

    int forward = position.second > piece->getPosition().second;
    weight += 100*forward;

    int distanceToEnemyFlag = abs(position.second - 9);
    weight += 100/(distanceToEnemyFlag+1);

    for (const auto& enemy : opponentPlayer->getKnown()) {
        int distance = abs(position.first - enemy->getPosition().first) +
                       abs(position.second - enemy->getPosition().second);

        if (distance == 0) {
            if (piece->getValue() > enemy->getValue()) {
                weight += 300;
            } else {
                weight -= 100;
            }
        }

        else if (distance == 1) {
            if (piece->getValue() > enemy->getValue()) {
                weight += 150;
            } else {
                weight -= 50;
            }
        }
    }

    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            std::pair<int, int> piecePosition = {x, y};
            Pieces* pieceName = game->getBoard()->getPiece(piecePosition);
            if (pieceName != nullptr && game->isEnemy(pieceName)) {
                int distToFlag = abs(flagPosition.first - x) + abs(flagPosition.second - y);
                if (distToFlag <= 2) {
                    int currentDistanceToFlag = abs(piece->getPosition().first - flagPosition.first) +
                                                abs(piece->getPosition().second - flagPosition.second);
                    int newDistanceToFlag = abs(position.first - flagPosition.first) +
                                            abs(position.second - flagPosition.second);

                    if (newDistanceToFlag < currentDistanceToFlag) {
                        weight += 300;
                    }
                }
            }
        }
    }

    cout << "Piece at " << piece->getPosition().first << "," << piece->getPosition().second
         << " to " << position.first << "," << position.second << " has weight " << weight << endl;
    return weight;
}

std::vector<int> ai::HeuristicAI::calculateMove(Game* game) {
    std::vector<int> positions;
    int bestWeight = INT_MIN;
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