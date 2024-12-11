#include <iostream>

// The following lines are here to check that SFML is installed and working
#include <client/PlayerController.h>
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}
// end of test SFML

#include <state.h>

using namespace std;
using namespace state;

/*int main() {
    auto game = Game::getInstance();
    game->startGame();
}*/

/*int main() {
// Initialisation du State
    // Supposons que vous ayez un Board singleton et des joueurs init
    state::Board* board = state::Board::getInstance();
    state::Player* player1 = new state::Player(); // Ajustez selon votre constructeur
    state::Player* player2 = new state::Player(); // idem
    std::vector<state::Player*> players = {player1, player2};

    // Initialisation de l'Engine avec le Board et les Players
    engine::engine gameEngine(board, players);

    // Création d’un PlayerController pour le joueur 0 (humain)
    client::PlayerController controller(&gameEngine, 0);
    controller.switchToHuman();

    // Affichage initial du plateau
    std::cout << "Initial Board State: \n";
    board->displayBoard(*player1); // Méthode hypothétique pour afficher le plateau du point de vue du joueur 1

    // Lance un tour
    std::cout << "Player 0, it's your turn.\n";
    // Le PlayerController demande à l’utilisateur des coordonnées
    MoveCoordinates coords = controller.getUserInput();

    // Pour exécuter le mouvement, soit vous construisez une commande, soit l'Engine
    // a une méthode pour déplacer directement une pièce:
    // Par exemple :
    // std::string command = "MOVE " + std::to_string(coords.startX) + " " + std::to_string(coords.startY) + " "
    //                     + std::to_string(coords.endX) + " " + std::to_string(coords.endY);
    // controller.executeCmd(command);

    // Ou si l'Engine a une méthode dédiée :
    bool success = gameEngine.movePiece(player1, coords.startX, coords.startY, coords.endX, coords.endY);
    if (!success) {
        std::cout << "Move failed! Try another move.\n";
    } else {
        std::cout << "Move succeeded!\n";
    }

    // Affichage après le mouvement
    std::cout << "Board State after move:\n";
    board->displayBoard(*player1);

    // Si vous avez une boucle de jeu, vous pourriez continuer ici.
    // Par exemple, alterner entre player 0 et player 1, ou demander un autre input.

    // Nettoyage (si nécessaire, selon votre gestion mémoire)
    delete player1;
    delete player2;
    // Si Board est un singleton géré statiquement, pas besoin de delete.

    return 0;}*/
