//
// Created by matthieu on 06/01/25.
//
#include "Render.h"
#include <iostream>

using namespace client;
std::string getProjectRootDirectory() {
    // Full path to the current file
    std::string filePath = __FILE__;

    // Find the "Stratego" directory in the path
    std::size_t pos = filePath.find("Stratego_Berdij_Charret_Preys_Pillot");
    if (pos == std::string::npos) {
        throw std::runtime_error("Unable to find 'Stratego_Berdij_Charret_Preys_Pillot' in the path");
    }

    // Extract the path up to and including "Stratego"
    return filePath.substr(0, pos + std::string("Stratego_Berdij_Charret_Preys_Pillot").length());
}

std::string constructPath(const std::string& relativePath) {
    return getProjectRootDirectory() + "/" + relativePath;
}

Render::Render(int cellSize)
        : window(sf::VideoMode(cellSize * 10, cellSize * 10), "Stratego - Plateau"),
          cellSize(cellSize), rows(10), cols(10) {
    // Charger l'image de fond
    std::string filepath = constructPath("src/client/img_render/board.png");
    if (!boardTexture.loadFromFile(filepath)) {
        std::cerr << "Erreur : Impossible de charger board.png" << std::endl;
    }
    boardSprite.setTexture(boardTexture);
    boardSprite.setScale(
        static_cast<float>(cellSize * cols) / boardTexture.getSize().x,
        static_cast<float>(cellSize * rows) / boardTexture.getSize().y
    ); // Redimensionner l'image de fond pour remplir la fenêtre
}

void Render::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Render::drawBoard() {
    window.draw(boardSprite); // Afficher l'image de fond
}

void Render::drawGrid() {
    sf::RectangleShape line; // Ligne pour dessiner la grille
    line.setFillColor(sf::Color::Black);

    // Dessiner les lignes horizontales
    for (int i = 0; i <= rows; ++i) {
        line.setSize(sf::Vector2f(window.getSize().x, 1)); // Largeur de la fenêtre, hauteur 1
        line.setPosition(0, i * cellSize);
        window.draw(line);
    }

    // Dessiner les lignes verticales
    for (int j = 0; j <= cols; ++j) {
        line.setSize(sf::Vector2f(1, window.getSize().y)); // Hauteur de la fenêtre, largeur 1
        line.setPosition(j * cellSize, 0);
        window.draw(line);
    }
}

void Render::drawCoordinates() {
    sf::Font font;
    std::string filepath = constructPath("src/client/fonts/arial.ttf");
    if (!font.loadFromFile(filepath)) { // Charge une police pour les textes
        std::cerr << "Erreur : Impossible de charger arial.ttf" << std::endl;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);

    // Dessiner les coordonnées sur les lignes
    for (int i = 0; i < rows; ++i) {
        text.setString(std::to_string(i));
        text.setPosition(5, i * cellSize + 5); // Décalage pour la lisibilité
        window.draw(text);
    }

    for (int j = 0; j < cols; ++j) {
        text.setString(std::to_string(j));
        text.setPosition(j * cellSize + 5, 5); // Décalage pour la lisibilité
        window.draw(text);
    }
}

void Render::run() {
    while (window.isOpen()) {
        handleEvents();
        window.clear();
        drawBoard();
        drawGrid();
        drawCoordinates();
        window.display();
    }
}
