//
// Created by Souhaila.B on 04/12/2024.
//
#include <SFML/Graphics.hpp> // Inclus les fonctionnalités de SFML

int main() {
    // Créer une fenêtre SFML avec une taille de 800x600 pixels et un titre
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test");

    // Boucle principale pour garder la fenêtre ouverte
    while (window.isOpen()) {
        sf::Event event;
        // Gérer les événements SFML (fermer la fenêtre par exemple)
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Fermer la fenêtre si l'utilisateur clique sur "fermer"
            }
        }

        // Effacer l'écran avec une couleur (noir ici)
        window.clear(sf::Color::Black);

        // Afficher le contenu dans la fenêtre
        window.display();
    }

    return 0;
}
