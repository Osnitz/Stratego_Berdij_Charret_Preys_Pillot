
#include "Render.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace client;

Render::Render(engine::Engine* engine, ScenarioManager* scenarioManager)
        : engine(engine), scenarioManager(scenarioManager), window(sf::VideoMode(800, 600), "Stratego - Interface") {
    // Charger les textures
    sf::Texture boardTexture;
    if (!boardTexture.loadFromFile("src/client/img_pieces/board.png")) {
        std::cerr << "Erreur : Impossible de charger board.png" << std::endl;
    }
    boardSprite.setTexture(boardTexture);

    // Charger les textures des pièces
    for (auto type : {state::PieceType::Scout, state::PieceType::Miner, state::PieceType::Flag}) {
        sf::Texture texture;
        std::string filename = "src/client/img_pieces/" + std::to_string(type) + ".png";
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Erreur : Impossible de charger " << filename << std::endl;
        }
        textures[type] = texture;
    }
}

void Render::run() {
    while (window.isOpen()) {
        handleEvents();
        window.clear();
        drawBoard();
        drawPieces();
        window.display();
    }
}

void Render::handleEvents() {
    sf::Event event;  // Initialisation correcte sans arguments
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = sf::Mouse::getPosition(window); // Utilisation correcte de getPosition
            int row = mousePos.y / 60; // Exemple d'échelle
            int col = mousePos.x / 60;
            std::cout << "Clic détecté sur la case (" << row << ", " << col << ")" << std::endl;
        }
    }
}

void Render::drawBoard() {
    window.draw(boardSprite);
}

void Render::drawPieces() {
    if (!engine) {
        std::cerr << "Erreur : Engine non initialisé" << std::endl;
        return;
    }

    auto game = engine->getGame(); // Récupérer le jeu via Engine
    if (!game) {
        std::cerr << "Erreur : Game non initialisé" << std::endl;
        return;
    }

    auto grid = game->getBoard()->getGrid(); // Récupérer la grille
    if (!grid) {
        std::cerr << "Erreur : Grid non disponible" << std::endl;
        return;
    }

    // Parcourir la grille
    for (size_t i = 0; i < grid->size(); ++i) { // Utilisation de '->' car 'grid' est un pointeur
        auto& row = (*grid)[i]; // Récupération de la ligne
        for (size_t j = 0; j < row.size(); ++j) {
            auto piece = row[j]; // Accès à une pièce spécifique
            if (piece) { // Vérifier si une pièce est présente
                sf::Sprite sprite;
                sprite.setTexture(textures[piece->getType()]); // Appeler 'getType()' sur l'objet pièce
                sprite.setPosition(j * 60, i * 60); // Exemple d'échelle
                window.draw(sprite);
            }
        }
    }
}
