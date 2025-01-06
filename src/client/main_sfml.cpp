//
// Created by matthieu on 06/01/25.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include "client.h"

using namespace client;

int display_test() {
    // Création d'une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fenêtre SFML");

    // Définir une limite de framerate (optionnel)
    window.setFramerateLimit(60);

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Gestion des événements
            if (event.type == sf::Event::Closed) {
                window.close(); // Fermer la fenêtre
            }
        }

        // Effacer l'écran
        window.clear(sf::Color::Black);

        // Dessiner des éléments ici si nécessaire

        // Afficher le contenu à l'écran
        window.display();
    }

    return 0;
}
int main(void)
{
    printf("hello\n");
    Render render(60); // Fenêtre 800x600, cases de 60 pixels
    render.run();
    return 0;

}