//
// Created by plt on 13/11/24.
//
/*#include <iostream>

#include "state.h"
#include "engine.h"

using namespace std;
using namespace state;


void testConstructor() {
    Pieces piece(1, "Soldat", 0, 0);
    Pieces miner(2, "miner", 0, 0);
    Pieces flag(0, "flag", 0, 0);
    int x=piece.getPosition().first;
    int y=piece.getPosition().second;
    if(piece.getValue()==1&&piece.getName()=="Soldat"&&x==0&&y==0&&piece.getRange()==1&&miner.getRange()==10&&flag.getRange()==0) {
        cout << "Contructor test passed!" << endl;
    }else {
        std::cerr << "Constructor test failed!" << std::endl;
    }
}

void testCheckBoard() {
    Pieces piece(1, "Soldat", 0, 0);
    pair<int, int> outOfBoard={-1,10};
    pair<int, int> inTheLake={4,2};
    pair<int, int> goodPosition={4,1};
    if(!piece.CheckBoard(outOfBoard,false)&&!piece.CheckBoard(inTheLake,false)&&piece.CheckBoard(goodPosition,false)) {
        cout << "CheckBoard test passed!" << endl;
    }else {
        cerr << "CheckBoard test failed!" << endl;
    }
}

void testSetPosition() {
    Board *board = Board::getInstance();
    Pieces piece(1, "Soldat", 0, 0);
    Pieces piece2(1, "Soldat", 1, 0);
    pair<int, int> newPosition={1,0};
    board->setPieceOnBoard(&piece);
    board->setPieceOnBoard(&piece2);
    piece.setPosition(newPosition);
    if(piece.getPosition()==newPosition &&board->getPiece({0,0})==nullptr&&board->getPiece(newPosition)==&piece) {
        std::cout << "setPosition test passed!" << std::endl;
    }else {
        std::cerr << "setPosition test failed!" << std::endl;
    }
    board->removeFromBoard(&piece);
}

void testCheckCase() {
    Board *board = Board::getInstance();
    auto game = Game::getInstance();
    game->switchTurn();
    Player *player=game->getCurrentPlayer();
    Pieces piece(1, "Soldat", 0, 0);
    Pieces miner(2, "miner", 1, 0);
    player->addPiece(&miner);
    board->setPieceOnBoard(&piece);
    board->setPieceOnBoard(&miner);
    if(piece.CheckCase({0,1})=="Empty" && piece.CheckCase({1,0})=="Ally" && miner.CheckCase({0,1})=="Enemy") {
        std::cout << "CheckCase test passed!" << std::endl;
    }else {
        std::cerr << "CheckCase test failed!" << std::endl;
    }
    board->removeFromBoard(&piece);
    board->removeFromBoard(&miner);
}

void testAttack() {
    Board* board=Board::getInstance();
    Pieces bomb1(11,"Bomb",0,0);
    Pieces bomb2(11,"Bomb",0,1);
    Pieces miner(3,"Miner",1,0);
    Pieces scout1(2,"Scout",1,1);
    Pieces marshal(10,"Marshal",2,0);
    Pieces spy(1,"Spy",2,1);
    Pieces scout2(2,"Scout",2,2);
    Pieces scout3(2,"Scout",3,0);
    Pieces sergeant(4,"Sergeant",3,1);
    Pieces scout4(2,"Scout",3,2);
    Pieces lieutenant(5,"Lieutenant",3,3);
    board->setPieceOnBoard(&bomb1);
    board->setPieceOnBoard(&bomb2);
    board->setPieceOnBoard(&miner);
    board->setPieceOnBoard(&scout1);
    board->setPieceOnBoard(&marshal);
    board->setPieceOnBoard(&spy);
    board->setPieceOnBoard(&scout2);
    board->setPieceOnBoard(&scout3);
    board->setPieceOnBoard(&sergeant);
    board->setPieceOnBoard(&scout4);
    board->setPieceOnBoard(&lieutenant);
    scout1.attack({0,0});
    if(board->getPiece({0,0})==nullptr&&board->getPiece({1,1})==nullptr) {
        cout << "scout est mort de bomb1" << endl;
        miner.attack({0,1});
        if(board->getPiece({0,1})==&miner&&board->getPiece({1,0})==nullptr) {
            cout << "miner a attaque bomb2" << endl;
            spy.attack({2,0});
            if(board->getPiece({2,0})==&spy&&board->getPiece({2,1})==nullptr) {
                cout << "spy attack marechal" << endl;
                scout2.attack({3,0});
                if(board->getPiece({3,0})==nullptr&&board->getPiece({2,2})==nullptr) {
                    cout << "scout2 et 3 se battent" << endl;
                    sergeant.attack({3,2});
                    if(board->getPiece({3,1})==nullptr) {
                        cout << "sergeant n'est plus la" << endl;
                        if(board->getPiece({3,2})==&sergeant) {
                            cout << "sergeant est à la place de scout4" << endl;
                            sergeant.attack({3,3});
                            if(board->getPiece({3,2})==nullptr&&board->getPiece({3,3})==&lieutenant) {
                                cout << "Attack test passed!" << endl;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "Attack test failed!" << endl;
}

int main() {
    testConstructor();
    testCheckBoard();
    //testCheckRange();
    testSetPosition();
    testCheckCase();
    testAttack();
    return 0;
}*/

#include <boost/test/unit_test.hpp>
#include "state.h"
#include "engine.h"

using namespace state;

// Test du constructeur
BOOST_AUTO_TEST_CASE(testConstructor) {
    Pieces piece(1, "Soldat", 0, 0);
    Pieces miner(2, "miner", 0, 0);
    Pieces flag(0, "flag", 0, 0);

    int x = piece.getPosition().first;
    int y = piece.getPosition().second;

    BOOST_CHECK(piece.getValue() == 1);
    BOOST_CHECK(piece.getName() == "Soldat");
    BOOST_CHECK(x == 0 && y == 0);
    BOOST_CHECK(piece.getRange() == 1);
    BOOST_CHECK(miner.getRange() == 10);
    BOOST_CHECK(flag.getRange() == 0);
}

// Test de CheckBoard
BOOST_AUTO_TEST_CASE(testCheckBoard) {
    Pieces piece(1, "Soldat", 0, 0);

    std::pair<int, int> outOfBoard = {-1, 10};
    std::pair<int, int> inTheLake = {4, 2};
    std::pair<int, int> goodPosition = {4, 1};

    BOOST_CHECK(!piece.CheckBoard(outOfBoard, false));
    BOOST_CHECK(!piece.CheckBoard(inTheLake, false));
    BOOST_CHECK(piece.CheckBoard(goodPosition, false));
}

// Test de setPosition
BOOST_AUTO_TEST_CASE(testSetPosition) {
    Board *board = Board::getInstance();
    Pieces piece(1, "Soldat", 0, 0);
    Pieces piece2(1, "Soldat", 1, 0);

    std::pair<int, int> newPosition = {1, 0};

    board->setPieceOnBoard(&piece);
    board->setPieceOnBoard(&piece2);

    piece.setPosition(newPosition);

    BOOST_CHECK(piece.getPosition() == newPosition);
    BOOST_CHECK(board->getPiece({0, 0}) == nullptr);
    BOOST_CHECK(board->getPiece(newPosition) == &piece);

    board->removeFromBoard(&piece);
}

// Test de CheckCase
BOOST_AUTO_TEST_CASE(testCheckCase) {
    Board *board = Board::getInstance();
    auto game = Game::getInstance();
    game->switchTurn();

    Player *player = game->getCurrentPlayer();

    Pieces piece(1, "Soldat", 0, 0);
    Pieces miner(2, "miner", 1, 0);

    player->addPiece(&miner);

    board->setPieceOnBoard(&piece);
    board->setPieceOnBoard(&miner);

    BOOST_CHECK(piece.CheckCase({0, 1}) == "Empty");
    BOOST_CHECK(piece.CheckCase({1, 0}) == "Ally");
    BOOST_CHECK(miner.CheckCase({0, 0}) == "Enemy");

    board->removeFromBoard(&piece);
    board->removeFromBoard(&miner);
}

// Test de l'attaque
BOOST_AUTO_TEST_CASE(testAttack) {
    Board *board = Board::getInstance();

    Pieces bomb1(11, "Bomb", 0, 0);
    Pieces bomb2(11, "Bomb", 0, 1);
    Pieces miner(3, "Miner", 1, 0);
    Pieces scout1(2, "Scout", 1, 1);
    Pieces marshal(10, "Marshal", 2, 0);
    Pieces spy(1, "Spy", 2, 1);
    Pieces scout2(2, "Scout", 2, 2);
    Pieces scout3(2, "Scout", 3, 0);
    Pieces sergeant(4, "Sergeant", 3, 1);
    Pieces scout4(2, "Scout", 3, 2);
    Pieces lieutenant(5, "Lieutenant", 3, 3);

    board->setPieceOnBoard(&bomb1);
    board->setPieceOnBoard(&bomb2);
    board->setPieceOnBoard(&miner);
    board->setPieceOnBoard(&scout1);
    board->setPieceOnBoard(&marshal);
    board->setPieceOnBoard(&spy);
    board->setPieceOnBoard(&scout2);
    board->setPieceOnBoard(&scout3);
    board->setPieceOnBoard(&sergeant);
    board->setPieceOnBoard(&scout4);
    board->setPieceOnBoard(&lieutenant);

    scout1.attack({0, 0});
    BOOST_CHECK(board->getPiece({0, 0}) == nullptr);
    BOOST_CHECK(board->getPiece({1, 1}) == nullptr);

    miner.attack({0, 1});
    BOOST_CHECK(board->getPiece({0, 1}) == &miner);
    BOOST_CHECK(board->getPiece({1, 0}) == nullptr);

    spy.attack({2, 0});
    BOOST_CHECK(board->getPiece({2, 0}) == &spy);
    BOOST_CHECK(board->getPiece({2, 1}) == nullptr);

    scout2.attack({3, 0});
    BOOST_CHECK(board->getPiece({3, 0}) == nullptr);
    BOOST_CHECK(board->getPiece({2, 2}) == nullptr);

    sergeant.attack({3, 2});
    BOOST_CHECK(board->getPiece({3, 1}) == nullptr);
    BOOST_CHECK(board->getPiece({3, 2}) == &sergeant);

    sergeant.attack({3, 3});
    BOOST_CHECK(board->getPiece({3, 2}) == nullptr);
    BOOST_CHECK(board->getPiece({3, 3}) == &lieutenant);
}
