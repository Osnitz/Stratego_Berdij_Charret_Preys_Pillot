#include <iostream>
#include <stdexcept>
#include "state.h"
#include "engine.h"

using namespace state;

void testSingletonInstance() {
    Board* board1 = Board::getInstance();
    Board* board2 = Board::getInstance();
    if (board1 != board2) {
        std::cerr << "SingletonInstance test failed!" << std::endl;
    } else {
        std::cout << "SingletonInstance test passed!" << std::endl;
    }
}

void testInitializeGrid() {
    Board* board = Board::getInstance();
    bool passed = true;
    for (std::size_t row = 0; row < 10; ++row) {
        for (std::size_t col = 0; col < 10; ++col) {
            if (board->getPiece({row, col}) != nullptr) {
                passed = false;
                break;
            }
        }
    }
    if (!passed) {
        std::cerr << "InitializeGrid test failed!" << std::endl;
    } else {
        std::cout << "InitializeGrid test passed!" << std::endl;
    }
}

void testGetPieceValidPosition() {
    Board* board = Board::getInstance();
    auto piece = new Pieces(4,"Soldat",0,0);
    board->setPieceOnBoard(piece);
    if (board->getPiece({0, 0}) != piece) {
        std::cerr << "GetPieceValidPosition test failed!" << std::endl;
    } else {
        std::cout << "GetPieceValidPosition test passed!" << std::endl;
    }
    board->removeFromBoard(piece);
    delete piece;
}

void testGetPieceInvalidPosition() {
    Board* board = Board::getInstance();
    bool passed = true;
    try {
        board->getPiece({-1, 0});
        passed = false;
    } catch (const std::out_of_range&) {}
    try {
        board->getPiece({0, -1});
        passed = false;
    } catch (const std::out_of_range&) {}
    try {
        board->getPiece({10, 0});
        passed = false;
    } catch (const std::out_of_range&) {}
    try {
        board->getPiece({0, 10});
        passed = false;
    } catch (const std::out_of_range&) {}
    if (!passed) {
        std::cerr << "GetPieceInvalidPosition test failed!" << std::endl;
    } else {
        std::cout << "GetPieceInvalidPosition test passed!" << std::endl;
    }
}

void testDisplayBoard() {
    Board* board = Board::getInstance();
    auto piece = new Pieces(4,"Soldat2",0,1);
    auto piece2 = new Pieces(4,"Soldat3",0,0);
    Player player;
    player.addPiece(piece);
    board->setPieceOnBoard(piece);
    board->setPieceOnBoard(piece2);
    std::cout << "DisplayBoard test output:" << std::endl;
    board->displayBoard(player);
}

int main() {
    testSingletonInstance();
    testInitializeGrid();
    testGetPieceValidPosition();
    testGetPieceInvalidPosition();
    testDisplayBoard();
    return 0;
}