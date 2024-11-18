//
// Created by estelle on 14/10/24.
//

#include "Player.h"
#include "Pieces.h"
#include "Board.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Game.h"

using namespace state;

Player :: Player() {
    std::vector<Pieces*> myPieces;
    std::vector<Pieces*>  capturedPieces= {nullptr};
    std::vector<Pieces*>  knownPieces;
}

Player:: ~Player(){};


void Player::chargeConfig(std::string fileName){
    Game *game = Game::getInstance();
    Board *board = Board::getInstance();
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "File " << fileName << " not found " << std::endl;
    }
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> dataline;

        while (std::getline(ss, cell, ',')) {
            cell.erase(std::remove(cell.begin(), cell.end(), '"'), cell.end());
            dataline.push_back(cell);
        }

        std::string name = dataline.at(0);
        int value = std::stoi(dataline.at(1));
        int x = std::stoi(dataline.at(2));
        int y = std::stoi(dataline.at(3));


        if (game->getCurrentPlayer() == game->getPlayer1()) {
            Pieces* piece = new Pieces(value, name, x, y);
            myPieces.push_back(piece);
            board->setPieceOnBoard(piece);
        }
        else {
            Pieces* piece = new Pieces(value, name, 9 - x, y);
            myPieces.push_back(piece);
            board->setPieceOnBoard(piece);
        }
    }
    board->displayBoard(*game->getCurrentPlayer());

}

void Player::removePiece(Pieces* piece) {
    if (myPieces.empty()) {
        std::cerr<<"No piece left"<<std::endl;
        return;
    }
    for (std::size_t i = 0; i < myPieces.size(); i++) {
        if (myPieces[i] == piece) {
            myPieces.erase(myPieces.begin() + i);
            return;
        }
    }
    std::cerr<<"Can't remove this piece : it doesn't exist!"<<std::endl;
}

void Player::addPiece(Pieces* piece) {
    myPieces.push_back(piece);
}

void Player::addCaptured(Pieces *piece) {
    Board::getInstance()->removeFromBoard(piece);
    auto size=capturedPieces.size();
    int value=piece->getValue();
    for(std::size_t i=0;i<size;i++) {
        int myvalue=capturedPieces[i]->getValue();
        if(value<=myvalue) {
            capturedPieces.insert(capturedPieces.begin()+i,piece);
            return;
        }
    }
    capturedPieces.push_back(piece);
}

std::vector<Pieces*> Player:: getCaptured() {
    return capturedPieces;
}

std::vector<Pieces*> Player:: getMyPieces() {
    return myPieces;
}

bool Player::belongTo(Pieces* piece) {
    for (std::size_t i = 0; i < myPieces.size(); i++) {
        if (myPieces[i] == piece) {
            return true;
        }
    }
    return false;
}

void Player:: displayCaptured () {
    if(capturedPieces.empty()) {
        std::cerr<<"You have no captured pieces"<<std::endl;
        return;
    }
    auto size=capturedPieces.size();
    for(std::size_t i=0;i<size;i++) {
        std::cout << capturedPieces[i]->getName()<< std::endl;
    }
}