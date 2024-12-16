//
// Created by Souhaila.B on 21/10/2024.
//
#include "state.h"
#include <iostream>
#include <map>

using namespace state;
using namespace std;

Board::Board() {
    // Initialize a 10x10 grid with nullptr (representing empty spaces)
    grid.resize(10, vector<Pieces*>(10, nullptr));
}

Board::~Board() {
    delete instance;
}

void Board::displayBoard(Player &player) {
    // Print the first line of column's number in the grid
    cout << "    "; // Initial alignment for the first row of the grid
    for (size_t col = 0; col < grid[0].size(); ++col) {
        cout << "   " << col << "   "; // Adjusting the spacing for column numbers
    }
    cout << endl;

    // Display the bounding line under column numbers
    std::cout << "   "; // Initial alignment for dashes
    for (std::size_t col = 0; col < grid[0].size(); ++col) {
        std::cout << "-------"; // Horizontal delineation
    }
    std::cout << "-" << std::endl; // End of the delineation to the right

    // Print the grid with the pieces
    for (std::size_t row = 0; row < grid.size(); ++row) {
        std::cout << row << "  "; // Print line's numbers inside the grid
        for (std::size_t col = 0; col < grid[row].size(); ++col) {
            if ((row == 4 && (col == 2 || col == 3 || col == 6 || col == 7)) || (row == 5 && (col == 2 || col == 3 || col == 6 || col == 7))) {
                std::cout << "|  XX  "; // Lacs
            } else {
                Pieces* piece = grid[row][col];
                if (piece == nullptr) {
                    std::cout << "|      "; // Empty box
                } else if (player.belongTo(piece)) {
                    int value = piece->getValue();
                    if (value < 10) {
                        std::cout << "|  0" << piece->getValue() << "  ";
                    } else {
                        std::cout << "|  " << piece->getValue() << "  ";
                    }
                } else {
                    std::cout << "|  ??  "; // Enemy's piece
                }
            }
        }
        std::cout << "|" << std::endl;

        // Display the bounding line under each line
        std::cout << "   "; // Alignment for dashes
        for (std::size_t col = 0; col < grid[0].size(); ++col) {
            std::cout << "-------"; // Horizontal delineation
        }
        std::cout << "-" << std::endl; // End of the delineation to the right
    }
}

Pieces* Board::getPiece(std::pair<int, int> position) {
    int x = position.first;
    int y = position.second;

    if (x >= 0 && static_cast<std::size_t>(x) < grid.size() && y >= 0 && static_cast<std::size_t>(y) < grid[0].size()) {
        return grid[x][y];
    }
    return nullptr;;
}



vector<vector<Pieces * >>* Board::getGrid() {
    return &grid;
}