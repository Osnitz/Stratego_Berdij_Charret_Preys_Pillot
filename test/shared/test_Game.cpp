//
// Created by matthieu on 06/11/24.
//

#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include "engine.h"

using namespace state;
using namespace engine;

BOOST_AUTO_TEST_SUITE(GameTestSuite)

    // Test constructor and getters
    BOOST_AUTO_TEST_CASE(testConstructor)
    {
        Game* game = new Game();
        auto firstplayer = game->getCurrentPlayer();
        BOOST_CHECK(game->getPlayer1() != nullptr);
        BOOST_CHECK(game->getPlayer2() != nullptr);
        BOOST_CHECK(firstplayer != nullptr);
        BOOST_CHECK(game->getCurrentPlayer() == game->getPlayer1()||game->getCurrentPlayer()== game->getPlayer2());
        BOOST_CHECK(game->getBoard() != nullptr);

        //test both possibilities for the first player
        bool test = false;
        while (!test)
        {
            Game* game2 = new Game();
            if (game2->getCurrentPlayer() != firstplayer)
            {
                test = true;
            }
        }
        BOOST_CHECK(test);
    }

    // Test de la méthode switchTurn
    BOOST_AUTO_TEST_CASE(testSwitchTurn)
    {
        Game* game = new Game();
        Player* firstPlayer = game->getCurrentPlayer();

        // Test du changement de joueur
        game->switchTurn();
        Player* secondPlayer = game->getCurrentPlayer();
        BOOST_CHECK(secondPlayer != nullptr);
        BOOST_CHECK(firstPlayer != secondPlayer);

        // Test du retour au premier joueur
        game->switchTurn();
        BOOST_CHECK(firstPlayer == game->getCurrentPlayer());
    }

    BOOST_AUTO_TEST_CASE(testAddCaptured)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Pieces piece(1, Scout, 0, 0, player);
        Pieces miner(2, Miner, 0, 0, player);
        Pieces flag(0, Flag, 0, 0, player);
        game->addCaptured(&piece, player);
        auto& capturedPieces = player->getCaptured();
        BOOST_CHECK(capturedPieces[0] == &piece);
        game->addCaptured(&miner, player);
        game->addCaptured(&flag, player);
        BOOST_CHECK(capturedPieces[0] == &flag);
        BOOST_CHECK(capturedPieces[1] == &piece);
        BOOST_CHECK(capturedPieces[2] == &miner);
    }

    BOOST_AUTO_TEST_CASE(testRemovePiece)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        auto& mypiece = player->getMyPieces();
        Pieces piece(1, Scout, 0, 0, player);
        Pieces miner(2, Miner, 0, 0, player);
        game->addPiece(&piece, player);

        BOOST_TEST_MESSAGE("piece doesn't exist");
        game->removePiece(&miner, player);

        game->removePiece(&piece, player);
        BOOST_CHECK(mypiece.empty());
        BOOST_TEST_MESSAGE("my piece is empty:");
        game->removePiece(&miner, player);
    }

    BOOST_AUTO_TEST_CASE(testGetOpponent)
    {
        Game* game = new Game();
        Player* firstPlayer = game->getCurrentPlayer();
        BOOST_CHECK(firstPlayer!=game->getOpponent());
        game->switchTurn();
        BOOST_CHECK(firstPlayer==game->getOpponent());
    }

    BOOST_AUTO_TEST_CASE(testBelongTo)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Pieces piece(1, Scout, 0, 0, player);
        Pieces miner(2, Miner, 0, 0, player);
        game->addPiece(&piece, player);
        BOOST_CHECK(game->belongTo(&piece));
        BOOST_CHECK(!game->belongTo(&miner));
    }

    BOOST_AUTO_TEST_CASE(testAddPiece)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        auto& mypiece = player->getMyPieces();
        Pieces piece(1, Scout, 0, 0, player);
        game->addPiece(&piece, player);
        BOOST_CHECK(mypiece[0]==&piece);
    }

    //Test of loadConfig and displayBoard
    BOOST_AUTO_TEST_CASE(testLoadConfig)
    {
        Game* game = new Game();
        std::string filePath = "src/shared/state/config/Balance.csv";

        std::string test = "test.csv";
        BOOST_TEST_MESSAGE("Wrong file");
        game->loadConfig(test);

        BOOST_TEST_MESSAGE("Board first player");
        game->loadConfig(filePath);

        game->switchTurn();
        BOOST_TEST_MESSAGE("Board second player");
        game->loadConfig(filePath);
    }

    BOOST_AUTO_TEST_CASE(testLimitBoard)
    {
        Game* game = new Game();
        std::pair<int, int> outOfBoard = {0, 10};
        std::pair<int, int> inTheLake = {5, 6};
        std::pair<int, int> goodPos = {0, 0};

        BOOST_TEST_MESSAGE("Out of Board");
        game->limitBoard(outOfBoard, false);
        BOOST_CHECK(!game->limitBoard(outOfBoard,true));

        BOOST_TEST_MESSAGE("In the Lake");
        game->limitBoard(inTheLake, false);
        BOOST_CHECK(!game->limitBoard(inTheLake,true));

        BOOST_CHECK(game->limitBoard(goodPos,true));
    }

    BOOST_AUTO_TEST_CASE(testIsAlly)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Player* otherPlayer = new Player(2);
        Pieces piece(1, Scout, 0, 0, player);
        Pieces miner(2, Miner, 0, 0, otherPlayer);
        BOOST_CHECK(game->isAlly(&piece));
        BOOST_CHECK(!game->isAlly(&miner));
    }

    BOOST_AUTO_TEST_CASE(testIsEnemy)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Player* otherPlayer = new Player(2);
        Pieces piece(1, Scout, 0, 0, player);
        Pieces miner(2, Miner, 0, 0, otherPlayer);
        BOOST_CHECK(!game->isEnemy(&piece));
        BOOST_CHECK(game->isEnemy(&miner));
    }

    BOOST_AUTO_TEST_CASE(testIsEmpty)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Pieces piece(1, Scout, 0, 0, player);
        Pieces* empty = nullptr;
        BOOST_CHECK(!game->isEmpty(&piece));
        BOOST_CHECK(game->isEmpty(empty));
    }

    //Test of setPieceOnBoard and RemoveFromBoard
    BOOST_AUTO_TEST_CASE(testPieceOnBoard)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Pieces piece(1, Scout, 0, 0, player);
        Board* board = game->getBoard();
        auto& grid = *board->getGrid();

        game->setPieceOnBoard(&piece, 1, 0);
        BOOST_CHECK(grid[1][0]==&piece);

        //if the position in the piece changed it should remove it
        game->removeFromBoard(&piece);
        BOOST_CHECK(grid[1][0]==nullptr);
    }

    BOOST_AUTO_TEST_CASE(testPossiblePosition)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Player* player2 = new Player(2);
        Pieces piece(1, Scout, 1, 2, player);
        Pieces flag(0, Flag, 1, 6, player);
        Pieces miner(2, Miner, 0, 0, player2);
        Pieces* empty = nullptr;
        game->setPieceOnBoard(&piece, 1, 2);
        game->setPieceOnBoard(&flag, 1, 6);
        game->setPieceOnBoard(&miner, 3, 2);

        BOOST_CHECK(game->possiblePositions(empty).empty());
        BOOST_CHECK(game->possiblePositions(&flag).empty());

        auto pos = game->possiblePositions(&piece);
        BOOST_CHECK(pos.size()==8);

        std::vector<std::pair<int, int>> result;
        result.emplace_back(1, 1);
        result.emplace_back(1, 0);
        result.emplace_back(1, 3);
        result.emplace_back(1, 4);
        result.emplace_back(1, 5);
        result.emplace_back(0, 2);
        result.emplace_back(2, 2);
        result.emplace_back(3, 2);
        for (int i = 0; i < pos.size(); i++)
        {
            BOOST_CHECK(pos[i]==result[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(testSetCurrentPlayer)
    {
        Game* game = new Game();
        auto player1 = game->getPlayer1();
        auto player2 = game->getPlayer2();

        game->setCurrentPlayer(player1);
        BOOST_CHECK(game->getCurrentPlayer()==player1);

        game->setCurrentPlayer(player2);
        BOOST_CHECK(game->getCurrentPlayer()==player2);
    }

    BOOST_AUTO_TEST_CASE(testIsFlagCaptured)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        auto opponent = game->getOpponent();
        Pieces piece(1, Scout, 0, 0, player);
        Pieces flag(0, Flag, 1, 6, opponent);

        state::Game::addCaptured(&piece, player);
        BOOST_CHECK(!game->isFlagCaptured());

        game->addCaptured(&flag, opponent);
        BOOST_CHECK(game->isFlagCaptured());
    }

    BOOST_AUTO_TEST_CASE(testHasValidMove)
    {
        Game* game = new Game();
        auto player = game->getCurrentPlayer();
        Pieces piece(1, Scout, 1, 2, player);
        Pieces flag(0, Flag, 1, 6, player);

        game->addPiece(&flag, player);
        BOOST_CHECK(!game->hasValidMoves());

        game->addPiece(&piece, player);
        BOOST_CHECK(game->hasValidMoves());
    }

    BOOST_AUTO_TEST_CASE(TestGetPlayerByID)
    {
        Game* game = new Game();
        auto board = game->getBoard();
        auto player1 = game->getPlayer1();
        auto player2 = game->getPlayer2();
        auto playertest = game->getPlayerByID(0);
        BOOST_CHECK(playertest==player1);
        playertest = game->getPlayerByID(1);
        BOOST_CHECK(playertest==player2);

        delete game;
    }

BOOST_AUTO_TEST_CASE(TestClearBoard)
{
    // Création de l'objet Game
    Game* game = new Game();
    auto board = game->getBoard();
    auto grid = board->getGrid();

    // Vérification préliminaire que la grille est bien initialisée
    BOOST_REQUIRE(grid != nullptr);

    for (int i = 0; i < grid->size(); i++)
    {
        for (int j = 0; j < grid->at(i).size(); j++)
        {
            Pieces* piece = new Pieces(1, Scout, i, j, game->getCurrentPlayer());
            game->setPieceOnBoard(piece, i, j);
        }
    }

    game->clearBoard();

    for (int i = 0; i < grid->size(); i++)
    {
        for (int j = 0; j < grid->at(i).size(); j++)
        {
            BOOST_CHECK(grid->at(i).at(j) == nullptr);
        }
    }

    delete game;
}


BOOST_AUTO_TEST_SUITE_END()
