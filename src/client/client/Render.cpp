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
    auto cellPos = piece->getPosition();
    sf::CircleShape pieceShape(static_cast<float>(cellSize) / 3);
    pieceShape.setFillColor(sf::Color::Green); // Green color for allies


    sf::Vector2i cellPosVector(cellPos.second, cellPos.first);
    sf::Vector2i pixelPos = cellToPixel(cellPosVector);


    // Compute the offset to center the circle in the cell
    float offsetX = static_cast<float>(cellSize) / 2.0f - pieceShape.getRadius();
    float offsetY = static_cast<float>(cellSize) / 2.0f - pieceShape.getRadius();


    pieceShape.setPosition(
            static_cast<float>(pixelPos.x) + offsetX,
            static_cast<float>(pixelPos.y) + offsetY
    );


    window.draw(pieceShape);
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
