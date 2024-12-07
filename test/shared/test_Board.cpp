#define BOOST_TEST_MODULE BoardTest
#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include <iostream>

using namespace state;

BOOST_AUTO_TEST_SUITE(BoardTestSuite)

// Test du Singleton
BOOST_AUTO_TEST_CASE(testSingletonInstance) {
    Board* board1 = Board::getInstance();
    Board* board2 = Board::getInstance();
    BOOST_CHECK(board1 == board2);
}

// Test de l'initialisation de la grille (vide au départ)
BOOST_AUTO_TEST_CASE(testInitializeGrid) {
    Board* board = Board::getInstance();
    bool allEmpty = true;
    for (std::size_t row = 0; row < 10; ++row) {
        for (std::size_t col = 0; col < 10; ++col) {
            if (board->getPiece({static_cast<int>(row), static_cast<int>(col)}) != nullptr) {
                allEmpty = false;
            }
        }
    }
    BOOST_CHECK(allEmpty);
}

// Test de l'ajout et de la récupération d'une pièce valide
BOOST_AUTO_TEST_CASE(testSetAndGetPiece) {
    Board* board = Board::getInstance();
    auto piece = new Pieces(4, "Soldat", 1, 1); // Pièce à position (1,1)
    board->setPieceOnBoard(piece);
    BOOST_CHECK(board->getPiece({1, 1}) == piece);

    board->removeFromBoard(piece);
    BOOST_CHECK(board->getPiece({1, 1}) == nullptr);

    delete piece;
}

// Test de récupération d'une pièce à une position invalide
BOOST_AUTO_TEST_CASE(testGetPieceInvalidPosition) {
    Board* board = Board::getInstance();
    BOOST_CHECK_THROW(board->getPiece({-1, 0}), std::out_of_range);
    BOOST_CHECK_THROW(board->getPiece({0, -1}), std::out_of_range);
    BOOST_CHECK_THROW(board->getPiece({10, 0}), std::out_of_range);
    BOOST_CHECK_THROW(board->getPiece({0, 10}), std::out_of_range);
}

// Test d'affichage de la grille (sans vérifier l'affichage directement)
BOOST_AUTO_TEST_CASE(testDisplayBoard) {
    Board* board = Board::getInstance();
    auto piece = new Pieces(5, "General", 0, 0); // Pièce positionnée en (0,0)
    auto player = Player();
    player.addPiece(piece);
    board->setPieceOnBoard(piece);

    std::cout << "Affichage du plateau : " << std::endl;
    board->displayBoard(player);

    board->removeFromBoard(piece);
    delete piece;
}

BOOST_AUTO_TEST_SUITE_END()
