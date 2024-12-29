//
// Created by Souhaila.B on 29/12/2024.
//
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <map>

const int gridSize = 10;
const int cellSize = 60;

enum class PieceType {
    EMPTY = -1, BOMB = 11, SPY = 1, SCOUT = 2, MINER = 3,
    SERGEANT = 4, LIEUTENANT = 5, CAPTAIN = 6,
    MAJOR = 7, COLONEL = 8, GENERAL = 9, MARSHAL = 10, FLAG = 0
};



struct Piece {
    PieceType type;
    sf::Sprite sprite;
};

std::map<PieceType, int> pieceLimits = {
        {PieceType::MARSHAL, 1},
        {PieceType::GENERAL, 1},
        {PieceType::COLONEL, 2},
        {PieceType::MAJOR, 3},
        {PieceType::CAPTAIN, 4},
        {PieceType::LIEUTENANT, 4},
        {PieceType::SERGEANT, 4},
        {PieceType::MINER, 5},
        {PieceType::SCOUT, 8},
        {PieceType::SPY, 1},
        {PieceType::FLAG, 1},
        {PieceType::BOMB, 6}
};


/*int main() {
    sf::RenderWindow window(sf::VideoMode(gridSize * cellSize, gridSize * cellSize), "Stratego - Interface");

    sf::Texture boardTexture;
    if (!boardTexture.loadFromFile("/Users/Souhaila.B/Desktop/COURS/ENSEA 3A/SFML/assets/board.png")) {
        std::cerr << "Erreur de chargement de l'image du plateau." << std::endl;
        return -1;
    }
    sf::Sprite boardSprite(boardTexture);
    boardSprite.setScale(
            static_cast<float>(gridSize * cellSize) / boardTexture.getSize().x,
            static_cast<float>(gridSize * cellSize) / boardTexture.getSize().y
    );

    std::map<PieceType, sf::Texture> textures;
    std::vector<std::string> pieceNames = {
            "bomb", "spy", "scout", "miner", "sergeant", "lieutenant", "captain",
            "major", "colonel", "general", "marshal", "flag"
    };

    for (int i = 0; i <= 11; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile("/Users/Souhaila.B/Desktop/COURS/ENSEA 3A/SFML/assets/" + pieceNames[i] + ".png")) {
            std::cerr << "Erreur de chargement de l'image : " << pieceNames[i] << ".png" << std::endl;
            return -1;
        }
        textures[static_cast<PieceType>(i)] = texture;
    }

    std::vector<std::vector<Piece>> board(gridSize, std::vector<Piece>(gridSize, {PieceType::EMPTY, sf::Sprite()}));

    int currentPieceIndex = 0;  // Index de la pièce à placer (0 à 10)
    bool isPlacementPhase = true;  // Phase de placement active
    PieceType currentPieceType = static_cast<PieceType>(currentPieceIndex);

    Piece* selectedPiece = nullptr;
    sf::Vector2i selectedCell(-1, -1);
    bool isPieceSelected = false;

    sf::RectangleShape highlightRect(sf::Vector2f(cellSize, cellSize));
    highlightRect.setFillColor(sf::Color(255, 255, 0, 100));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (isPlacementPhase && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f scaledPos = window.mapPixelToCoords(mousePos);

                int x = static_cast<int>(scaledPos.x) / cellSize;
                int y = static_cast<int>(scaledPos.y) / cellSize;

                if (board[y][x].type == PieceType::EMPTY) {
                    board[y][x].type = currentPieceType;
                    board[y][x].sprite.setTexture(textures[currentPieceType]);
                    board[y][x].sprite.setPosition(x * cellSize, y * cellSize);
                    board[y][x].sprite.setScale(
                            static_cast<float>(cellSize) / textures[currentPieceType].getSize().x,
                            static_cast<float>(cellSize) / textures[currentPieceType].getSize().y
                    );

                    currentPieceIndex++;
                    if (currentPieceIndex > 11) {
                        isPlacementPhase = false;  // Passer à la phase de jeu
                    } else {
                        currentPieceType = static_cast<PieceType>(currentPieceIndex);
                    }
                }
            } else if (!isPlacementPhase && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f scaledPos = window.mapPixelToCoords(mousePos);

                int x = static_cast<int>(scaledPos.x) / cellSize;
                int y = static_cast<int>(scaledPos.y) / cellSize;

                if (isPieceSelected && selectedPiece != nullptr) {
                    if (selectedCell == sf::Vector2i(x, y)) {
                        isPieceSelected = false;
                        selectedPiece = nullptr;
                    } else if (board[y][x].type == PieceType::EMPTY) {
                        board[y][x].type = selectedPiece->type;
                        board[y][x].sprite = selectedPiece->sprite;
                        board[y][x].sprite.setPosition(x * cellSize, y * cellSize);

                        board[selectedCell.y][selectedCell.x].type = PieceType::EMPTY;
                        isPieceSelected = false;
                        selectedPiece = nullptr;
                    }
                } else if (board[y][x].type != PieceType::EMPTY) {
                    selectedPiece = &board[y][x];
                    selectedCell = sf::Vector2i(x, y);
                    isPieceSelected = true;
                }
            }
        }

        window.clear();
        window.draw(boardSprite);

        if (isPieceSelected) {
            highlightRect.setPosition(selectedCell.x * cellSize, selectedCell.y * cellSize);
            window.draw(highlightRect);
        }

        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                if (board[i][j].type != PieceType::EMPTY) {
                    window.draw(board[i][j].sprite);
                }
            }
        }

        window.display();
    }

    return 0;
}*/
