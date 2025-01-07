//
// Created by matthieu on 06/01/25.
//
#include "Render.h"
#include "state/Board.h"
#include "state/Pieces.h"
#include "state/Game.h"


#include <iostream>
#include <unistd.h>


using namespace client;
using namespace state;
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
        : window(sf::VideoMode(cellSize * 10, cellSize * 10), "Stratego - Plateau"),
          cellSize(cellSize), rows(10), cols(10), game(game) {
    std::string filepath = constructPath("src/client/img_render/board.png");
    if (!boardTexture.loadFromFile(filepath)) {
        std::cerr << "Erreur : Impossible de charger board.png" << std::endl;
    }
    boardSprite.setTexture(boardTexture);
    boardSprite.setScale(
            static_cast<float>(cellSize * cols) / static_cast<float>(boardTexture.getSize().x),
            static_cast<float>(cellSize * rows) / static_cast<float>(boardTexture.getSize().y)
    );
    // Charger les textures des pièces
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
            textures[pieceType] = std::move(texture);
        }
    }
}




void Render::handleEvents() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
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
    window.draw(boardSprite);
}




void Render::drawGrid() {
    sf::RectangleShape line;
    line.setFillColor(sf::Color::Black);


    // horizontal lines
    for (int i = 0; i <= rows; ++i) {
        line.setSize(sf::Vector2f(window.getSize().x, 1));//width of the window, height 1
        line.setPosition(0, i * cellSize);
        window.draw(line);
    }


    // Vertical lines
    for (int j = 0; j <= cols; ++j) {
        line.setSize(sf::Vector2f(1, window.getSize().y)); // height of the window, width 1
        line.setPosition(j * cellSize, 0);
        window.draw(line);
    }
}




void Render::drawCoordinates() {
    sf::Font font;
    std::string filepath = constructPath("src/client/fonts/arial.ttf");
    if (!font.loadFromFile(filepath)) { // Load the font
        std::cerr << "Erreur : Impossible de charger arial.ttf" << std::endl;
    }


    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);


    // Draw the row and column numbers
    for (int i = 0; i < rows; ++i) {
        text.setString(std::to_string(i));
        text.setPosition(5, i * cellSize + 5); // Shift for readability
        window.draw(text);
    }


    for (int j = 0; j < cols; ++j) {
        text.setString(std::to_string(j));
        text.setPosition(j * cellSize + 5, 5); // Shift for readability
        window.draw(text);
    }
}




sf::Vector2i Render::pixelToCell(const sf::Vector2i& pixelPos) {
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    int gridX = (int)worldPos.x / cellSize;
    int gridY = (int)worldPos.y / cellSize;
    return {gridX, gridY};
}




sf::Vector2i Render::cellToPixel(const sf::Vector2i& cellPos) {
    int px = cellPos.x * cellSize;
    int py = cellPos.y * cellSize;
    return {px, py};
}




void Render::drawHighlight(const sf::Vector2i& cellPos) {
    sf::RectangleShape highlight(sf::Vector2f(cellSize, cellSize));
    highlight.setFillColor(sf::Color(255, 255, 0, 100)); // semi-transparent yellow
    sf::Vector2i pixelPos = cellToPixel(cellPos);
    highlight.setPosition((float)pixelPos.x, (float)pixelPos.y);


    window.draw(highlight);
}




void Render::drawPiecesOnBoard(state::Game* game) {
    auto board = game->getBoard();
    const std::vector<std::vector<Pieces*>>* grid = board->getGrid();


    for (const auto& row : *grid) {
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
        window.clear();
        drawBoard();
        drawGrid();
        drawCoordinates();
        drawPiecesOnBoard(game);
        window.display();
        usleep(2000000);
        game->switchTurn();
    }
}




void Render::drawAllyPiece(Pieces* piece) {
    if (!piece) return;

    auto pieceType = piece->getType(); // Obtenir le type de la pièce
    auto it = textures.find(pieceType);    // Trouver la texture associée
    if (it == textures.end()) {
        std::cerr << "Erreur : Texture non trouvée pour le type de pièce " << static_cast<int>(pieceType) << std::endl;
        return;
    }

    sf::Sprite pieceSprite;
    pieceSprite.setTexture(it->second);

    // Calculer la position de la cellule
    auto cellPos = piece->getPosition();
    sf::Vector2i cellPosVector(cellPos.second, cellPos.first);
    sf::Vector2i pixelPos = cellToPixel(cellPosVector);

    // Redimensionner et positionner le sprite
    pieceSprite.setScale(
            static_cast<float>(cellSize) / pieceSprite.getTexture()->getSize().x,
            static_cast<float>(cellSize) / pieceSprite.getTexture()->getSize().y
    );
    pieceSprite.setPosition(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));

    window.draw(pieceSprite);
}




void Render::drawEnemyPiece(Pieces* piece) {
    auto cellPos = piece->getPosition();
    sf::CircleShape pieceShape(static_cast<float>(cellSize) / 3);
    pieceShape.setFillColor(sf::Color::Red); // Red color for enemies
    sf::Vector2i cellPosVector(cellPos.second, cellPos.first);
    sf::Vector2i pixelPos = cellToPixel(cellPosVector);


    float offsetX = static_cast<float>(cellSize) / 2.0f - pieceShape.getRadius();
    float offsetY = static_cast<float>(cellSize) / 2.0f - pieceShape.getRadius();


    pieceShape.setPosition(
            static_cast<float>(pixelPos.x) + offsetX,
            static_cast<float>(pixelPos.y) + offsetY
    );


    window.draw(pieceShape);
}
