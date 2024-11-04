//
// Created by Souhaila.B on 21/10/2024.
//
#include "Board.h"
#include <iostream>
#include <stdexcept> // Pour std::out_of_range

// Initialisation du pointeur singleton à nullptr
state::Board* state::Board::instance = nullptr;

// Constructeur privé
state::Board::Board() {
    // Initialiser une grille vide 10x10
    //grid.resize(10, std::vector<int>(10, 0)); // 0 représente une case vide
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

// Affiche la grille vide délimitée par des tirets et des barres verticales avec les numéros dans la grille
void state::Board::displayBoard(Player & player) {
    // Afficher la première ligne de numéros de colonnes dans la grille
    std::cout << "    "; // Alignement initial pour la première ligne de la grille
    for (int col = 0; col < grid[0].size(); ++col) {
        std::cout << "  " << col << "   "; // Ajustement de l'espacement pour les numéros de colonnes
    }
    std::cout << std::endl;

    // Afficher la ligne de délimitation sous les numéros de colonnes
    std::cout << "   "; // Alignement initial pour les tirets
    for (int col = 0; col < grid[0].size(); ++col) {
        std::cout << "------"; // Délimitation horizontale
    }
    std::cout << "-" << std::endl; // Fin de la délimitation à droite

    // Afficher la grille vide avec des délimitations et numéros de lignes
    for (int row = 0; row < grid.size(); ++row) {
        std::cout << row << "  "; // Afficher le numéro de ligne à l'intérieur de la grille
        for (int col = 0; col < grid[row].size(); ++col) {
            std::cout << "|     "; // Cases vides avec des barres verticales
        }
        std::cout << "|" << std::endl;

        // Afficher la ligne de délimitation sous chaque ligne
        std::cout << "   "; // Alignement pour les tirets
        for (int col = 0; col < grid[0].size(); ++col) {
            std::cout << "------"; // Délimitation horizontale
        }
        std::cout << "-" << std::endl; // Fin de la délimitation à droite
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