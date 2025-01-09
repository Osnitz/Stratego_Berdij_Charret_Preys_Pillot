//
// Created by matthieu on 06/01/25.
//
#include "Render.h"
#include "../../shared/state/Board.h"
#include "../../shared/state/Pieces.h"
#include "../../shared/state/Game.h"
#include "../../shared/state/Player.h"
#include <iostream>
#include <../shared/ai/RandomAI.h>


using namespace client;
using namespace state;
using namespace render;

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


Render::Render(int cellSize, state::Game* game)
        : cellSize(cellSize), rows(10), cols(10), game(game) {
    // Ajouter une marge pour les pièces capturées à gauche et à droite
    const int marginCapturedPieces = 160;

    // Définir la taille de la fenêtre en tenant compte de la marge pour les pièces capturées
    window.create(sf::VideoMode(cellSize * cols + 2* marginCapturedPieces, cellSize * rows), "Stratego - Plateau");

    // Stocker la taille initiale de la fenêtre
    initialWindowSize = window.getSize();

    // Charger le plateau
    std::string filepath = constructPath("src/client/img_render/board.png");
    if (!boardTexture.loadFromFile(filepath)) {
        std::cerr << "Erreur : Impossible de charger board.png" << std::endl;
    }
    boardSprite.setTexture(boardTexture);
    boardSprite.setScale(
            static_cast<float>(cellSize * cols) / static_cast<float>(boardTexture.getSize().x),
            static_cast<float>(cellSize * rows) / static_cast<float>(boardTexture.getSize().y)
    );

    // Chargement des textures pour les pièces
    std::map<state::PieceType, std::string> texturePaths = {
            {state::PieceType::Bomb, "src/client/img_render/bomb.png"},
            {state::PieceType::Captain, "src/client/img_render/captain.png"},
            {state::PieceType::Colonel, "src/client/img_render/colonel.png"},
            {state::PieceType::Flag, "src/client/img_render/flag.png"},
            {state::PieceType::General, "src/client/img_render/general.png"},
            {state::PieceType::Lieutenant, "src/client/img_render/lieutenant.png"},
            {state::PieceType::Major, "src/client/img_render/major.png"},
            {state::PieceType::Marshal, "src/client/img_render/marshal.png"},
            {state::PieceType::Miner, "src/client/img_render/miner.png"},
            {state::PieceType::Scout, "src/client/img_render/scout.png"},
            {state::PieceType::Sergeant, "src/client/img_render/sergeant.png"},
            {state::PieceType::Spy, "src/client/img_render/spy.png"},
    };

    for (const auto& [pieceType, path] : texturePaths) {
        sf::Texture texture;
        if (!texture.loadFromFile(constructPath(path))) {
            std::cerr << "Erreur : Impossible de charger " << path << std::endl;
        } else {
            textures[pieceType] = texture;
        }
    }
}



void Render::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::Resized) {
            // Ajuster la vue pour couvrir toute la fenêtre redimensionnée
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2i cellPos = pixelToCell(pixelPos);

                std::cout << "Clic sur la case : "
                          << cellPos.x << ", " << cellPos.y << std::endl;
            }
        }
    }
}




void Render::drawBoard() {
    // Ajuster la position et la taille du sprite pour couvrir uniquement la zone du plateau
    boardSprite.setPosition(0, 0); // La zone de jeu commence à (0, 0)
    boardSprite.setScale(
            static_cast<float>(cellSize * cols) / static_cast<float>(boardTexture.getSize().x),
            static_cast<float>(cellSize * rows) / static_cast<float>(boardTexture.getSize().y)
    );

    // Dessiner uniquement dans la zone du plateau
    window.draw(boardSprite);
}



void Render::drawGrid() {
    sf::RectangleShape line;
    line.setFillColor(sf::Color::Black);

    // Définir les limites du plateau (sans les marges des pièces capturées)
    int gridWidth = cellSize * cols;  // Largeur du plateau
    int gridHeight = cellSize * rows; // Hauteur du plateau

    // Lignes horizontales
    for (int i = 0; i <= rows; ++i) {
        line.setSize(sf::Vector2f(gridWidth, 1)); // Largeur limitée au plateau, hauteur 1
        line.setPosition(0, i * cellSize);       // Positionner les lignes dans le plateau
        window.draw(line);
    }

    // Lignes verticales
    for (int j = 0; j <= cols; ++j) {
        line.setSize(sf::Vector2f(1, gridHeight)); // Hauteur limitée au plateau, largeur 1
        line.setPosition(j * cellSize, 0);        // Positionner les lignes dans le plateau
        window.draw(line);
    }
}



void Render::drawCoordinates() {
    sf::Font font;
    std::string filepath = constructPath("src/client/fonts/arial.ttf");
    if (!font.loadFromFile(filepath)) { // Charger la police
        std::cerr << "Erreur : Impossible de charger arial.ttf" << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);

    // Limiter le dessin des numéros de ligne et de colonne à la zone du plateau
    for (int i = 0; i < rows; ++i) {
        text.setString(std::to_string(i));
        text.setPosition(5, i * cellSize + 5); // Décalage pour la lisibilité
        if (i * cellSize < rows * cellSize) { // S'assurer que c'est dans les limites du plateau
            window.draw(text);
        }
    }

    for (int j = 0; j < cols; ++j) {
        text.setString(std::to_string(j));
        text.setPosition(j * cellSize + 5, 5); // Décalage pour la lisibilité
        if (j * cellSize < cols * cellSize) { // S'assurer que c'est dans les limites du plateau
            window.draw(text);
        }
    }
}



sf::Vector2i Render::pixelToCell(const sf::Vector2i& pixelPos) {
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    int gridX = static_cast<int>(worldPos.x) / cellSize;
    int gridY = static_cast<int>(worldPos.y) / cellSize;
    return {gridX, gridY};
}


sf::Vector2i Render::cellToPixel(const sf::Vector2i& cellPos) {
    int px = cellPos.x * cellSize;
    int py = cellPos.y * cellSize;
    return {px, py};
}


void Render::highlightPossiblePositions(const std::vector<std::pair<int, int>>& positions) {
    sf::Color highlightColor(0, 255, 0, 100); // Semi-transparent green
    for (const auto& pos : positions) {
        highlightOnePiece(pos, highlightColor); // Réutilisation de highlightOnePiece
    }
}



void Render::highlightOnePiece(const std::pair<int, int> selectedPosition, sf::Color color)
{
    sf::RectangleShape highlight(sf::Vector2f(cellSize, cellSize));
    highlight.setFillColor(color);
    sf::Vector2i pixelPos = cellToPixel({selectedPosition.second, selectedPosition.first});
    highlight.setPosition(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));

    window.draw(highlight);
}


void Render::drawPiecesOnBoard(state::Game* game) {
    auto board = game->getBoard();
    const std::vector<std::vector<Pieces*>>* grid = board->getGrid();


    for (const auto& row : *grid) {
        drawCapturedPieces();
        for (Pieces* piece : row) {
            if (piece) {
                if (game->belongTo(piece)) {
                    drawAllyPiece(piece);
                } else {
                    drawEnemyPiece(piece);
                }
            }
        }
    }
}


void Render::run() {
    while (window.isOpen()) {
        handleEvents();

        // Rectangle de fond blanc couvrant toute la fenêtre
        sf::RectangleShape fullBackground(sf::Vector2f(window.getSize().x, window.getSize().y));
        fullBackground.setFillColor(sf::Color(255, 255, 255));
        window.clear();
        window.draw(fullBackground); // Dessiner le fond blanc

        // Dessiner les éléments du jeu
        drawBoard();
        drawGrid();
        drawCoordinates();
        drawPiecesOnBoard(game);
        drawCapturedPieces();

        window.display();
    }
}

void Render::drawCapturedPieces() {
    const int margin = 5; // Réduit la marge entre les pièces et les bords
    const int offsetY = 20; // Réduit l'espacement vertical entre les lignes
    const int maxCapturedPerRow = 7; // Chaque ligne peut contenir jusqu'à 10 pièces capturées
    const int capturedAreaWidth = maxCapturedPerRow * (cellSize / 2) + 2 * margin; // Zone fixe pour les pièces capturées

    // Rectangle blanc pour le fond des pièces capturées
    sf::RectangleShape background(sf::Vector2f(capturedAreaWidth, cellSize * rows));
    background.setFillColor(sf::Color(255, 255, 255)); // Fond blanc
    background.setPosition(cellSize * cols, 0); // Toujours aligné à droite du plateau
    window.draw(background);

    auto player1Captured = game->getPlayer1()->getCaptured();
    auto player2Captured = game->getPlayer2()->getCaptured();

    // Dessiner les pièces capturées du joueur 1 en bas à droite
    for (size_t i = 0; i < player1Captured.size(); ++i) {
        Pieces* piece = player1Captured[i];
        if (!piece) continue;

        sf::Sprite pieceSprite;
        auto pieceType = piece->getType();
        auto it = textures.find(pieceType);
        if (it != textures.end()) {
            pieceSprite.setTexture(it->second);
        } else {
            std::cerr << "Erreur : Texture non trouvée pour le type de pièce " << static_cast<int>(pieceType) << std::endl;
            continue;
        }

        int row = i / maxCapturedPerRow;
        int col = i % maxCapturedPerRow;

        sf::Vector2i capturedPos(
        cellSize * cols + margin + col * (cellSize / 1.4), // Réduction de l'espacement horizontal
                cellSize * rows - (row + 1) * (cellSize / 2 + offsetY) // Réduction de l'espacement vertical
        );

        pieceSprite.setScale(
                static_cast<float>(cellSize) / (1.3 * pieceSprite.getTexture()->getSize().x), // Réduction de la taille
                static_cast<float>(cellSize) / (1.3 * pieceSprite.getTexture()->getSize().y)
        );
        pieceSprite.setPosition(static_cast<float>(capturedPos.x), static_cast<float>(capturedPos.y));
        window.draw(pieceSprite);
    }

    // Dessiner les pièces capturées du joueur 2 en haut à droite
    for (size_t i = 0; i < player2Captured.size(); ++i) {
        Pieces* piece = player2Captured[i];
        if (!piece) continue;

        sf::Sprite pieceSprite;
        auto pieceType = piece->getType();
        auto it = textures.find(pieceType);
        if (it != textures.end()) {
            pieceSprite.setTexture(it->second);
        } else {
            std::cerr << "Erreur : Texture non trouvée pour le type de pièce " << static_cast<int>(pieceType) << std::endl;
            continue;
        }

        int row = i / maxCapturedPerRow;
        int col = i % maxCapturedPerRow;

        sf::Vector2i capturedPos(
        cellSize * cols + margin + col * (cellSize / 1.4), // Réduction de l'espacement horizontal
                margin + row * (cellSize / 2 + offsetY) // Réduction de l'espacement vertical
        );

        pieceSprite.setScale(
                static_cast<float>(cellSize) / (1.3* pieceSprite.getTexture()->getSize().x), // Réduction de la taille
                static_cast<float>(cellSize) / (1.3 * pieceSprite.getTexture()->getSize().y)
        );
        pieceSprite.setPosition(static_cast<float>(capturedPos.x), static_cast<float>(capturedPos.y));
        window.draw(pieceSprite);
    }
}







void Render::drawAllyPiece(Pieces* piece) {
    if (!piece) return;

    auto pieceType = piece->getType();
    auto it = textures.find(pieceType);
    if (it == textures.end()) {
        std::cerr << "Erreur : Texture non trouvée pour le type de pièce " << static_cast<int>(pieceType) << std::endl;
        return;
    }

    sf::Sprite pieceSprite;
    pieceSprite.setTexture(it->second);

    auto cellPos = piece->getPosition();
    sf::Vector2i cellPosVector(cellPos.second, cellPos.first);
    sf::Vector2i pixelPos = cellToPixel(cellPosVector);

    // Resize the sprite to fit the cell
    pieceSprite.setScale(
            static_cast<float>(cellSize) / pieceSprite.getTexture()->getSize().x,
            static_cast<float>(cellSize) / pieceSprite.getTexture()->getSize().y
    );
    pieceSprite.setPosition(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));

    window.draw(pieceSprite);
}


void Render::drawEnemyPiece(Pieces* piece) {
    static sf::Texture unknownTexture;
    static bool isTextureLoaded = false;

    if (piece->isRevealed())
    {
        drawAllyPiece(piece);
        highlightOnePiece(piece->getPosition(), sf::Color(255, 0, 0, 150));
        piece->setReveal(false);
    }
    else
    {
        // Load the texture if it hasn't been loaded yet
        if (!isTextureLoaded) {
            std::string filepath = constructPath("src/client/img_render/inconnu3.png");
            if (!unknownTexture.loadFromFile(filepath)) {
                std::cerr << "Erreur : Impossible de charger inconnu3.png" << std::endl;
                return;
            }
            isTextureLoaded = true;
        }

        auto cellPos = piece->getPosition();
        sf::Vector2i cellPosVector(cellPos.second, cellPos.first);
        sf::Vector2i pixelPos = cellToPixel(cellPosVector);

        sf::Sprite sprite;
        sprite.setTexture(unknownTexture);
        sprite.setScale(
                static_cast<float>(cellSize) / unknownTexture.getSize().x,
                static_cast<float>(cellSize) / unknownTexture.getSize().y
        );
        sprite.setPosition(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));

        window.draw(sprite);
    }

}



sf::RenderWindow* Render::getWindow() {
    return &window;
}


std::vector<int> Render::getPlayerInput() {
    sf::Vector2i selectedCell(-1, -1);
    sf::Vector2i targetCell(-1, -1);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return {-1, -1, -1, -1};
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i clickedCell = pixelToCell({event.mouseButton.x, event.mouseButton.y});

                    if (selectedCell.x == -1) {
                        selectedCell = clickedCell;

                        std::pair<int, int> selectedPair(selectedCell.y, selectedCell.x);
                        Pieces* selectedPiece = game->getBoard()->getPiece(selectedPair);
                        if (!selectedPiece) {
                            std::cout << "Aucune pièce à cette position !" << std::endl;
                            selectedCell = {-1, -1};
                            continue;
                        }
                        if (game->belongTo(selectedPiece)) {
                            std::vector<std::pair<int, int>> possiblePositions = game->possiblePositions(selectedPiece);
                            // Redraw the board with the selected piece and possible positions highlighted
                            window.clear(sf::Color(255, 255, 255)); // Fond blanc
                            drawBoard();
                            drawGrid();
                            drawCoordinates();
                            drawPiecesOnBoard(game);
                            highlightOnePiece(selectedPair, sf::Color(255, 255, 0, 150)); // jaune
                            highlightPossiblePositions(possiblePositions);
                            window.display();
                        }
                    } else {
                        targetCell = clickedCell;
                        return {selectedCell.y, selectedCell.x, targetCell.y, targetCell.x};
                    }
                }
            }
        }
    }
    return {-1, -1, -1, -1}; // Retour par défaut si la fenêtre est fermée
}


void Render::displayEndScreen(int winnerPlayerID) {
    sf::Font font;
    std::string fontPath = constructPath("src/client/fonts/arial.ttf");
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Erreur : Impossible de charger la police arial.ttf" << std::endl;
        return;
    }

    sf::Text endText;
    endText.setFont(font);
    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::White);
    endText.setString("Player " + std::to_string(winnerPlayerID) + " wins!");
    endText.setPosition(
            static_cast<float>(initialWindowSize.x) / 2 - endText.getLocalBounds().width / 2,
            static_cast<float>(initialWindowSize.y) / 2 - endText.getLocalBounds().height / 2
    );

    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    window.clear();
    window.draw(boardSprite);
    window.draw(overlay);
    window.draw(endText);
    window.display();

    sf::Event event{};
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                return;
            }
        }
    }
}

int Render::displayDifficultySelection(const std::string& titleText) {
    sf::Font font;
    std::string fontPath = constructPath("src/client/fonts/arial.ttf");

    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Erreur : Impossible de charger la police arial.ttf");
    }

    // Création du titre
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setString(titleText);

    // Calcul initial pour centrer le titre (ne change pas si la fenêtre est redimensionnée)
    title.setPosition(
            static_cast<float>(initialWindowSize.x) / 2 - title.getLocalBounds().width / 2,
            50
    );



    // Création des boutons
    sf::Text easyButton, mediumButton, hardButton;
    easyButton.setFont(font);
    easyButton.setCharacterSize(30);
    easyButton.setFillColor(sf::Color::White);
    easyButton.setString("Easy");

    mediumButton.setFont(font);
    mediumButton.setCharacterSize(30);
    mediumButton.setFillColor(sf::Color::White);
    mediumButton.setString("Medium");

    hardButton.setFont(font);
    hardButton.setCharacterSize(30);
    hardButton.setFillColor(sf::Color::White);
    hardButton.setString("Hard");

    // Positions initiales des boutons
    easyButton.setPosition(100, 150);
    mediumButton.setPosition(100, 250);
    hardButton.setPosition(100, 350);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return -1; // Retourne -1 si la fenêtre est fermée
            } else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Convertir les coordonnées du pixel en coordonnées du monde
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                if (easyButton.getGlobalBounds().contains(worldPos)) {
                    return 1; // Facile
                } else if (mediumButton.getGlobalBounds().contains(worldPos)) {
                    return 2; // Moyen
                } else if (hardButton.getGlobalBounds().contains(worldPos)) {
                    return 3; // Difficile
                }
            }
        }

        // Dessin des éléments
        window.clear();
        window.draw(title);
        window.draw(easyButton);
        window.draw(mediumButton);
        window.draw(hardButton);
        window.display();
    }

    return -1; // Si la boucle se termine sans choix
}

ScenarioParameters Render::displayInitializationScreen() {
    sf::Font font;
    std::string fontPath = constructPath("src/client/fonts/arial.ttf");

    auto scenarioParameters = ScenarioParameters(PVP, nullptr, nullptr);

    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Erreur : Impossible de charger la police arial.ttf");
    }

    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::White);
    title.setString("Choose Game Mode");
    title.setPosition(
            static_cast<float>(window.getSize().x) / 2 - title.getLocalBounds().width / 2,
            50
    );

    sf::Text pvpButton, pveButton, aiButton;
    pvpButton.setFont(font);
    pvpButton.setCharacterSize(30);
    pvpButton.setFillColor(sf::Color::White);
    pvpButton.setString("Player vs Player");
    pvpButton.setPosition(100, 150);

    pveButton.setFont(font);
    pveButton.setCharacterSize(30);
    pveButton.setFillColor(sf::Color::White);
    pveButton.setString("Player vs AI");
    pveButton.setPosition(100, 250);

    aiButton.setFont(font);
    aiButton.setCharacterSize(30);
    aiButton.setFillColor(sf::Color::White);
    aiButton.setString("AI vs AI");
    aiButton.setPosition(100, 350);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return scenarioParameters;
            } else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Convertir les coordonnées de pixel en coordonnées de monde
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                if (pvpButton.getGlobalBounds().contains(worldPos)) {
                    return scenarioParameters;
                }
                if (pveButton.getGlobalBounds().contains(worldPos)) {
                    scenarioParameters.mode = PVE;
                    int difficulty = displayDifficultySelection("Choix du niveau de l'IA");
                    switch (difficulty) {
                        case 1:
                            scenarioParameters.aiModule0 = new ai::RandomAI();
                            break;
                        case 2:
                            scenarioParameters.aiModule0 = new ai::RandomAI();
                            break;
                        case 3:
                            scenarioParameters.aiModule0 = new ai::RandomAI();
                            break;
                        default:
                            scenarioParameters.aiModule0 = new ai::RandomAI();
                    }
                    return scenarioParameters;
                }
                if (aiButton.getGlobalBounds().contains(worldPos)) {
                    scenarioParameters.mode = AIvsAI;
                    int difficulty = displayDifficultySelection("Choix du niveau de l'IA 1");
                    switch (difficulty) {
                        case 1:
                            scenarioParameters.aiModule0 = new ai::RandomAI();
                            break;
                        case 2:
                            scenarioParameters.aiModule0 = new ai::RandomAI();
                            break;
                        case 3:
                            scenarioParameters.aiModule0 = new ai::RandomAI();
                            break;
                        default:
                            scenarioParameters.aiModule1 = new ai::RandomAI();
                    }
                    int difficulty2 = displayDifficultySelection("Choix du niveau de l'IA 2");
                    switch (difficulty2) {
                        case 1:
                            scenarioParameters.aiModule1 = new ai::RandomAI();
                            break;
                        case 2:
                            scenarioParameters.aiModule1 = new ai::RandomAI();
                            break;
                        case 3:
                            scenarioParameters.aiModule1 = new ai::RandomAI();
                            break;
                        default:
                            scenarioParameters.aiModule1 = new ai::RandomAI();
                    }
                    return scenarioParameters;
                }
            }
        }

        window.clear();
        window.draw(title);
        window.draw(pvpButton);
        window.draw(pveButton);
        window.draw(aiButton);
        window.display();
    }

    return scenarioParameters;
}

std::string Render::displayConfigurationSelection(const std::string& titleText) {
    sf::Font font;
    std::string fontPath = constructPath("src/client/fonts/arial.ttf");

    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Erreur : Impossible de charger la police arial.ttf");
    }

    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(30);
    title.setFillColor(sf::Color::White);
    title.setString(titleText);
    title.setPosition(
            static_cast<float>(initialWindowSize.x) / 2 - title.getLocalBounds().width / 2,
            50
    );



    sf::Text defensiveButton, offensiveButton, balancedButton;
    defensiveButton.setFont(font);
    defensiveButton.setCharacterSize(30);
    defensiveButton.setFillColor(sf::Color::White);
    defensiveButton.setString("Defensive");
    defensiveButton.setPosition(100, 150);

    offensiveButton.setFont(font);
    offensiveButton.setCharacterSize(30);
    offensiveButton.setFillColor(sf::Color::White);
    offensiveButton.setString("Offensive");
    offensiveButton.setPosition(100, 250);

    balancedButton.setFont(font);
    balancedButton.setCharacterSize(30);
    balancedButton.setFillColor(sf::Color::White);
    balancedButton.setString("Balanced");
    balancedButton.setPosition(100, 350);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            } else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                if (defensiveButton.getGlobalBounds().contains(worldPos)) {
                    return constructPath("src/shared/state/config/Defensive.csv");
                }
                if (offensiveButton.getGlobalBounds().contains(worldPos)) {
                    return constructPath("src/shared/state/config/Offensive.csv");
                }
                if (balancedButton.getGlobalBounds().contains(worldPos)) {
                    return constructPath("src/shared/state/config/Balance.csv");
                }
            }
        }

        window.clear();

        // Dessiner les autres éléments par-dessus
        window.draw(title);
        window.draw(defensiveButton);
        window.draw(offensiveButton);
        window.draw(balancedButton);
        window.display();
    }

    return ""; // Si la boucle se termine sans choix
}



