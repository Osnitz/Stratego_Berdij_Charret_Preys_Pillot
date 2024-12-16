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

Board::~Board() = default;

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