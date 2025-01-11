//
// Created by plt on 18/12/24.
//

#include "ai.h"
#include "AdvancedAI.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <deque>
#include <vector>
#include <unordered_map>
#include <state/Board.h>
#include <state/Game.h>
#include <state/PieceType.h>
#include <state/Pieces.h>
#include <state/Player.h>

using namespace state;
using namespace std;

ai::AdvancedAI::AdvancedAI() {
    unordered_map<Pieces*, vector<pair<int, int>>> possibleMovesCache;
    std::unordered_map<Pieces*, std::deque<std::pair<int, int>>> previousPositions;
}

ai::AdvancedAI::~AdvancedAI() = default;

void ai::AdvancedAI::clearCache() {
    possibleMovesCache.clear();
}

vector<pair<int, int>> ai::AdvancedAI::getPossibleMoves(Pieces* piece, Game* game) {
    if (possibleMovesCache.find(piece) == possibleMovesCache.end()) {
        auto moves = game->possiblePositions(piece);
        possibleMovesCache[piece] = moves;
    }
    return possibleMovesCache[piece];
}

void ai::AdvancedAI::updatePreviousPositions(Pieces* piece, const std::pair<int, int>& newPosition) {
    if (std::find(previousPositions[piece].begin(), previousPositions[piece].end(), newPosition) != previousPositions[piece].end()) {
        return;
    }

    if (previousPositions[piece].size() >= 5) {
        previousPositions[piece].erase(previousPositions[piece].begin());
    }

    previousPositions[piece].push_back(newPosition);
}

int ai::AdvancedAI::calculateGamePhase(Game* game) {
    int totalMyPieces = game->getCurrentPlayer()->getMyPieces().size();
    int totalOpponentPieces = game->getOpponent()->getMyPieces().size();

    if (totalMyPieces > 20 && totalOpponentPieces > 20) {
        return 0;
    } else if (totalMyPieces > 10 && totalOpponentPieces > 10) {
        return 1;
    } else {
        return 2;
    }
}

int ai::AdvancedAI::advancedCalculator(Pieces* piece, const std::pair<int, int>& position, Game* game) {
    int weight = 0;
    auto currentPlayer = game->getCurrentPlayer();
    auto opponentPlayer = game->getOpponent();
    int totalMyPieces = currentPlayer->getMyPieces().size();
    Board* board = game->getBoard();
    std::pair<int, int> flagPosition = {-1, -1};
    updatePreviousPositions(piece, position);
    const auto& history = previousPositions[piece];
    int gamePhase = calculateGamePhase(game);

    switch (gamePhase) {
        case 0:
            weight += 100;
        break;
        case 1:
            weight += 50;
        break;
        case 2:
            weight -= 50;
        break;
    }

    for (const auto& pieces : currentPlayer->getMyPieces()) {
        if (pieces->getType() == Flag) {
            flagPosition = pieces->getPosition();
            break;
        }
    }

    if (position.second > piece->getPosition().second) {
        weight += 100;
    }

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

    if (piece->getType() == Miner && totalMyPieces > 10) {
        weight -= 50;
    }

    if (piece->getType() == Scout) {
        if (totalMyPieces > 25) {
            weight += 100;
        } else if (totalMyPieces < 10) {
            weight += 200;
        }
    }

    if (piece->getType() != Scout && piece->getType() != Miner) {
        if (totalMyPieces > 25) {
            weight += 30;
        } else if (totalMyPieces < 10) {
            weight += 50;
        } else {
            weight += 20;
        }
    }

    if (history.size() >= 3) {
        if (history[history.size() - 1] == position &&
            history[history.size() - 2] == piece->getPosition() &&
            history[history.size() - 3] == history[history.size() - 1]) {
            weight -= 50;
            }
    }

    std::pair<int, int> currentPos = position;
    int blockWeight = 100;
    while (true) {
        std::pair<int, int> posAlly = {currentPos.first, currentPos.second + 1};
        Pieces* allyPiece = board->getPiece(posAlly);
        if (allyPiece != nullptr && game->isAlly(allyPiece)) {
            weight += blockWeight;
            blockWeight += 50;
            currentPos = posAlly;
        } else {
            break;
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

    int distanceToFlag = abs(flagPosition.first - position.first) + abs(flagPosition.second - position.second);
    if (piece->getPosition().second < 2 && distanceToFlag <= 2) {
        weight += 150;
    }

    if (position.second >= 7) {
        weight += 200;
    }

    cout << "Piece at " << piece->getPosition().first << "," << piece->getPosition().second
         << " to " << position.first << "," << position.second << " has weight " << weight << endl;
    return weight;
}

/*vector<std::pair<std::pair<int, int>, int>> ai::AdvancedAI::weightedRanking(Pieces* piece, Game* game) {
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

vector<Pieces*> ai::AdvancedAI::getPlayablePieces(Game* game) {
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

void ai::AdvancedAI::immobileCounting(state::Game* game) {
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

std::vector<int> ai::AdvancedAI::calculateMove(Game* game) {
    std::vector<int> positions;
    int bestWeight = INT_MIN;
    Pieces* bestPiece = nullptr;
    std::pair<int, int> bestPosition = {-1, -1};

    auto playablePieces = getPlayablePieces(game);

    for (auto piece : playablePieces) {

        auto possibleMoves=getPossibleMoves(piece,game);

        for (auto& move : possibleMoves) {
            int weight=advancedCalculator(piece,move,game);
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

