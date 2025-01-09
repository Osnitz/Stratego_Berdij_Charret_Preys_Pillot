//
// Created by plt on 18/12/24.
//

#include "ai.h"
#include "HeuristicAI.h"
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

ai::HeuristicAI::HeuristicAI() {
    unordered_map<Pieces*, vector<pair<int, int>>> possibleMovesCache;
    std::unordered_map<Pieces*, std::deque<std::pair<int, int>>> previousPositions;
}

ai::HeuristicAI::~HeuristicAI() = default;

void ai::HeuristicAI::clearCache() {
    possibleMovesCache.clear();
}

vector<pair<int, int>> ai::HeuristicAI::getPossibleMoves(Pieces* piece, Game* game) {
    if (possibleMovesCache.find(piece) == possibleMovesCache.end()) {
        auto moves = game->possiblePositions(piece);
        possibleMovesCache[piece] = moves;
        //cout << "Piece at " << piece->getPosition().first << "," << piece->getPosition().second << " has " << moves.size() << " possible moves." << endl;
    }
    return possibleMovesCache[piece];
}

void ai::HeuristicAI::updatePreviousPositions(Pieces* piece, const std::pair<int, int>& newPosition) {
    if (std::find(previousPositions[piece].begin(), previousPositions[piece].end(), newPosition) != previousPositions[piece].end()) {
        return;
    }

    if (previousPositions[piece].size() >= 5) {
        previousPositions[piece].erase(previousPositions[piece].begin());  // Enlever l'élément le plus ancien
    }

    previousPositions[piece].push_back(newPosition);  // Ajouter la nouvelle position
}

int ai::HeuristicAI::calculateGamePhase(Game* game) {
    int totalMyPieces = game->getCurrentPlayer()->getMyPieces().size();
    int totalOpponentPieces = game->getOpponent()->getMyPieces().size();

    if (totalMyPieces > 20 && totalOpponentPieces > 20) {
        return 0;  // Début de la partie (phase offensive)
    } else if (totalMyPieces > 10 && totalOpponentPieces > 10) {
        return 1;  // Milieu de la partie (phase stratégique)
    } else {
        return 2;  // Fin de la partie (phase défensive)
    }
}

int ai::HeuristicAI::heuristicCalculator(Pieces* piece, const std::pair<int, int>& position, Game* game) {
    int weight = 0;
    auto currentPlayer = game->getCurrentPlayer();
    auto opponentPlayer = game->getOpponent();
    int totalMyPieces = currentPlayer->getMyPieces().size();
    Board* board = game->getBoard();
    std::pair<int, int> flagPosition = {-1, -1};
    updatePreviousPositions(piece, position);
    const auto& history = previousPositions[piece];
    int gamePhase = calculateGamePhase(game);

    // Calcul des poids en fonction de la phase de jeu
    switch (gamePhase) {
        case 0:  // Début de la partie : favorise les explorations
            weight += 100;
        break;
        case 1:  // Milieu de la partie : privilégier la sécurité et l'avancement
            weight += 50;
        break;
        case 2:  // Fin de la partie : jouer plus défensivement
            weight -= 50;
        break;
    }
    // Section 1: Identifier la position du drapeau
    for (const auto& pieces : currentPlayer->getMyPieces()) {
        if (pieces->getType() == Flag) {
            flagPosition = pieces->getPosition();
            break;
        }
    }

    // Section 2: Priorité d'avancer vers l'ennemi
    if (position.second > piece->getPosition().second) {
        weight += 100;
    }

    int distanceToEnemyFlag = abs(position.second - 9);
    weight += 100/(distanceToEnemyFlag+1);


    // Section 3: Capturer les ennemis avec des pièces plus fortes
    for (const auto& enemy : opponentPlayer->getKnown()) {
        int distance = abs(position.first - enemy->getPosition().first) +
                       abs(position.second - enemy->getPosition().second);

        // Si on est sur la même case, capturer en fonction de la valeur
        if (distance == 0) {
            if (piece->getValue() > enemy->getValue()) {
                weight += 300; // Capturer un ennemi avec une pièce plus forte
            } else {
                weight -= 100; // Eviter la capture avec une pièce plus faible
            }
        }
        // Si à distance de 1, essayer de capturer
        else if (distance == 1) {
            if (piece->getValue() > enemy->getValue()) {
                weight += 150; // Priorité à la capture si on est plus fort
            } else {
                weight -= 50; // Eviter de s'exposer à une capture
            }
        }
    }

    // Section 4: Priorité pour les mineurs à la fin de la partie
    if (piece->getType() == Miner && totalMyPieces > 10) {
        weight -= 50; // Priorité basse en début/milieu de partie
    }

    // Section 5: Priorité des Scouts en début et fin de partie
    if (piece->getType() == Scout) {
        if (totalMyPieces > 25) { // Début de partie
            weight += 100;  // Favoriser l'exploration
        } else if (totalMyPieces < 10) { // Fin de partie
            weight += 200;  // Prioriser la capture du drapeau
        }
    }

    // Section 6: Priorité des pièces ordinaires (autres que Scout et Miner) en milieu de partie
    if (piece->getType() != Scout && piece->getType() != Miner) {
        if (totalMyPieces > 25) {
            weight += 30;  // Favoriser l'avancement si c'est le début du jeu
        } else if (totalMyPieces < 10) {
            weight += 50;  // Favoriser l'avancement dans le territoire ennemi en fin de partie
        } else {
            weight += 20;  // Normal en milieu de partie
        }
    }

    // Section 7: Bloquer les mouvements répétitifs
    if (history.size() >= 3) {  // Vérifier les trois derniers mouvements
        // Vérifier si on est revenu à une position précédente dans les 3 derniers mouvements
        if (history[history.size() - 1] == position &&
            history[history.size() - 2] == piece->getPosition() &&
            history[history.size() - 3] == history[history.size() - 1]) {
            weight -= 50;  // Pénaliser les mouvements répétitifs
            }
    }

    // Section 8: Débloquer les pièces bloquées par des alliés
    std::pair<int, int> currentPos = position;
    int blockWeight = 100;  // Poids initial pour débloquer
    while (true) {
        std::pair<int, int> posAlly = {currentPos.first, currentPos.second + 1};
        Pieces* allyPiece = board->getPiece(posAlly);
        if (allyPiece != nullptr && game->isAlly(allyPiece)) {
            weight += blockWeight;
            blockWeight += 50;  // Augmenter le poids pour chaque niveau de blocage
            currentPos = posAlly;  // Continuer avec la pièce bloquante suivante
        } else {
            break;  // Arrêter lorsque la chaîne de blocage est terminée
        }
    }

    if (piece->getType() == Miner || piece->getType() == Flag) {
        // Si la pièce est vulnérable (ex: mineur, drapeau), chercher des pièces ennemies proches
        for (int x = 0; x < 10; ++x) {
            for (int y = 0; y < 10; ++y) {
                std::pair<int, int> enemyPos = {x, y};
                Pieces* enemy = game->getBoard()->getPiece(enemyPos);
                if (enemy != nullptr && game->isEnemy(enemy)) {
                    int dist = abs(position.first - x) + abs(position.second - y);
                    if (dist <= 2) {  // Si un ennemi est trop proche
                        weight += 100; // Pénaliser ou augmenter la priorité pour la défense
                    }
                }
            }
        }
    }

    // Section 9: Encourager les pièces qui s'approchent du drapeau
    int distanceToFlag = abs(flagPosition.first - position.first) + abs(flagPosition.second - position.second);
    if (piece->getPosition().second < 2 && distanceToFlag <= 2) {
        weight += 150;  // Priorité pour se rapprocher du drapeau
    }

    // Section 10: Privilégier les pièces avec un Y plus élevé (plus près de l'ennemi)
    if (position.second >= 7) {  // Plus proche de la ligne du fond
        weight += 200;
    }

    cout << "Piece at " << piece->getPosition().first << "," << piece->getPosition().second
         << " to " << position.first << "," << position.second << " has weight " << weight << endl;
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

