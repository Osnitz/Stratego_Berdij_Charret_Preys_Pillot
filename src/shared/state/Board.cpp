//
// Created by Souhaila.B on 21/10/2024.
//
#include "Board.h"
#include <iostream>
#include <map>

#include "Game.h"

using namespace state;

Board* Board::instance = nullptr;

Board::Board() {
    // Initialize a 10x10 grid with nullptr (representing empty spaces)
    grid.resize(10, std::vector<Pieces*>(10, nullptr));
}

Board::~Board() {
    delete instance;
}

Board* Board::getInstance() {
    if (!instance) {
        instance = new Board();
    }
    return instance;
}

void Board::displayBoard(Player &player) {
    // Afficher la première ligne de numéros de colonnes dans la grille
    std::cout << "    "; // Alignement initial pour la première ligne de la grille
    for (std::size_t col = 0; col < grid[0].size(); ++col) {
        std::cout << "  " << col << "   "; // Ajustement de l'espacement pour les numéros de colonnes
    }
    std::cout << std::endl;

    // Afficher la ligne de délimitation sous les numéros de colonnes
    std::cout << "   "; // Alignement initial pour les tirets
    for (std::size_t col = 0; col < grid[0].size(); ++col) {
        std::cout << "------"; // Délimitation horizontale
    }
    std::cout << "-" << std::endl; // Fin de la délimitation à droite

    // Afficher la grille avec les pièces
    for (std::size_t row = 0; row < grid.size(); ++row) {
        std::cout << row << "  "; // Afficher le numéro de ligne à l'intérieur de la grille
        for (std::size_t col = 0; col < grid[row].size(); ++col) {
            Pieces* piece = grid[row][col];
            if (piece == nullptr) {
                std::cout << "|     "; // Case vide
            } else if (player.belongTo(piece)) {
                std::cout << "|  " << piece->getValue() << "  "; // Pièce du joueur actuel
            } else {
                std::cout << "|  ?  "; // Pièce de l'adversaire
            }
        }
        std::cout << "|" << std::endl;

        // Afficher la ligne de délimitation sous chaque ligne
        std::cout << "   "; // Alignement pour les tirets
        for (std::size_t col = 0; col < grid[0].size(); ++col) {
            std::cout << "------"; // Délimitation horizontale
        }
        std::cout << "-" << std::endl; // Fin de la délimitation à droite
    }
}

Pieces* Board::getPiece(std::pair<int, int> position) {
    int x = position.first;
    int y = position.second;

    if (x >= 0 && static_cast<std::size_t>(x) < grid.size() && y >= 0 && static_cast<std::size_t>(y) < grid[0].size()) {
        return grid[x][y];
    } else {
        throw std::out_of_range("Invalid position on the board.");
    }
}

std::vector<std::vector<Pieces*>> Board::getgrid() {
    return grid;
}

void Board::setPieceOnBoard(Pieces* piece) {
    std::pair<int, int> position = piece->getPosition();
    grid[position.first][position.second] = piece;
}

void Board::removeFromBoard(Pieces* piece) {
    std::pair<int, int> position = piece->getPosition();
    grid[position.first][position.second] = nullptr;
}
