//
// Created by Souhaila.B on 21/10/2024.
//
#include "Board.h"
#include "Player.h"
#include <iostream>
#include <stdexcept> // Pour std::out_of_range

// Initialisation du pointeur singleton à nullptr
state::Board* state::Board::instance = nullptr;

// Constructeur privé
state::Board::Board() {
    // Initialiser une grille vide 10x10 (par exemple)
    grid.resize(10, std::vector<int>(10, 0)); // 0 représente une case vide
}

// Destructeur
state::Board::~Board() {
    delete instance;
}

// Méthode pour obtenir l'instance unique du singleton
state::Board* state::Board::getInstance() {
    if (!instance) {
        instance = new Board();
    }
    return instance;
}

// Affiche la grille
// Affiche la grille
// Affiche la grille
void state::Board::displayBoard() {
    // Afficher la ligne supérieure avec des numéros de colonnes
    std::cout << "   "; // Pour l'alignement des numéros de colonnes
    for (int col = 0; col < grid[0].size(); ++col) {
        std::cout << col << " "; // Afficher les numéros de colonne
    }
    std::cout << std::endl;

    // Afficher la grille avec des numéros de lignes
    for (int row = 0; row < grid.size(); ++row) {
        std::cout << row << " "; // Afficher le numéro de ligne
        for (int piece : grid[row]) {
            std::cout << piece << " "; // Afficher les éléments de la grille
        }
        std::cout << std::endl;
    }
}


/*
// Récupère la pièce à une position donnée
int state::Board::getPiece(std::pair<int, int> position) {
    int x = position.first;
    int y = position.second;

    if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size()) {
        return grid[x][y];
    } else {
        throw std::out_of_range("Position invalide sur la grille.");
    }
}
*/