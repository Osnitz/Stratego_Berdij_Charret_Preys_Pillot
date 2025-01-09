//
// Created by matthieu on 20/12/24.
//


#include <boost/test/included/unit_test.hpp>
#include <client/HumanController.h>
#include <client/PlayerController.h>

#include "state.h"
#include "engine.h"

using namespace state;
BOOST_AUTO_TEST_SUITE(EngineTestSuite)

BOOST_AUTO_TEST_CASE(TestConstructor)
{
    auto game = new Game();
    auto engine = new engine::Engine(game);

    BOOST_CHECK(engine->getGame() == game);

    delete engine;
    delete game;
}

BOOST_AUTO_TEST_CASE(TestGetGame)
{
    // Initialize the game and engine
    auto game = new Game();
    auto engine = new engine::Engine(game);

    // Verify that the game instance returned by getGame() is the same as the one passed to the Engine
    BOOST_CHECK(engine->getGame() == game);

    // Cleanup
    delete engine;
    delete game;
}


BOOST_AUTO_TEST_CASE(TestEndTurn)
{
    auto game = new Game();
    auto* engine = new engine::Engine(game);
    auto* previousPlayer = game->getCurrentPlayer();
    engine->endTurn();
    auto newPlayer = game->getCurrentPlayer();
    BOOST_CHECK(previousPlayer!=newPlayer);

    delete engine;
    delete game;
}

BOOST_AUTO_TEST_CASE(TestHandleCmdPlacement)
{
    auto game = new Game();
    auto engine = new engine::Engine(game);

    auto currentPlayer = game->getCurrentPlayer();
    auto controller = new client::HumanController(engine, nullptr);//used to correctly construct the path to the file
    std::string filePath = controller->constructPath("src/shared/state/config/Balance.csv");
    engine->handleCmdPlacement(filePath);
    auto myPieces = currentPlayer->getMyPieces();
    auto nextPlayer = game->getCurrentPlayer();

    BOOST_CHECK(currentPlayer!=nextPlayer);
    BOOST_CHECK(!myPieces.empty());

    delete engine;
    delete game;

}

BOOST_AUTO_TEST_CASE(TestHandleCmdMove)
{
    // Initialize the game and engine
    auto game = new Game();
    auto engine = new engine::Engine(game);
    auto board = game->getBoard();

    game->setCurrentPlayer(game->getPlayer1());

    // Create two pieces: one for the current player, one for the opponent
    auto myPiece = new Pieces(2, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(myPiece, game->getPlayer1());
    game->setPieceOnBoard(myPiece, 4, 0);

    auto opponentPiece = new Pieces(1, PieceType::Spy, 5, 0, game->getPlayer2());
    game->addPiece(opponentPiece, game->getPlayer2());
    game->setPieceOnBoard(opponentPiece, 5, 0);

    // Case 1: The piece does not belong to the current player
    BOOST_CHECK_MESSAGE(!engine->handleCmdMove({5, 0}, {5, 1}), "The piece should not belong to the current player.");
    game->setCurrentPlayer(game->getPlayer1());

    // Case 2: The destination is invalid
    BOOST_CHECK_MESSAGE(!engine->handleCmdMove({4, 0}, {4, -1}), "The destination is out of bounds.");
    game->setCurrentPlayer(game->getPlayer1());

    // Case 3: The destination is empty
    BOOST_CHECK_MESSAGE(engine->handleCmdMove({4, 0}, {4, 1}), "The move should be valid.");
    BOOST_CHECK_MESSAGE(board->getPiece({4, 1}) == myPiece, "The piece should have been moved.");

    // Reset the board state
    game->setCurrentPlayer(game->getPlayer1());
    game->removeFromBoard(myPiece);
    game->setPieceOnBoard(myPiece, 4, 0);

    // Case 4: The destination contains an opponent's piece -> Attack
    BOOST_CHECK_MESSAGE(engine->handleCmdMove({4, 0}, {5, 0}), "The attack should succeed.");
    BOOST_CHECK_MESSAGE(game->getPlayer1()->getCaptured().size() == 1, "The player should have captured one piece.");

    // Case 5: The destination contains an allied piece
    auto alliedPiece = new Pieces(3, PieceType::Scout, 4, 1, game->getPlayer1());
    game->addPiece(alliedPiece, game->getPlayer1());
    game->setPieceOnBoard(alliedPiece, 4, 1);

    BOOST_CHECK_MESSAGE(!engine->handleCmdMove({4, 0}, {4, 1}), "A piece cannot move to a tile containing an allied piece.");

    // Cleanup for allied piece
    game->removeFromBoard(alliedPiece);
    delete alliedPiece;

    // Cleanup for game objects
    delete engine;
    delete game;
    delete myPiece;
    delete opponentPiece;
}

BOOST_AUTO_TEST_CASE(TestAttackAllCases)
{
    // Initialize the game and engine
    auto game = new Game();
    auto engine = new engine::Engine(game);
    auto board = game->getBoard();

    game->setCurrentPlayer(game->getPlayer1());

    // Case 2: Attack a Bomb with a Miner
    auto bombPiece = new Pieces(11, PieceType::Bomb, 5, 0, game->getPlayer2());
    auto minerPiece = new Pieces(3, PieceType::Miner, 4, 0, game->getPlayer1());
    game->addPiece(bombPiece, game->getPlayer2());
    game->addPiece(minerPiece, game->getPlayer1());
    game->setPieceOnBoard(bombPiece, 5, 0);
    game->setPieceOnBoard(minerPiece, 4, 0);

    engine->attack(minerPiece, {5, 0});
    BOOST_CHECK_MESSAGE(game->getPlayer1()->getCaptured().size() == 1, "The Miner should have captured the Bomb.");
    BOOST_CHECK(board->getPiece({5, 0}) == minerPiece);

    game->removeFromBoard(minerPiece);
    game->removeFromBoard(bombPiece);

    // Case 2bis: Attack a Bomb with a non-Miner
    auto bombPiece2 = new Pieces(11, PieceType::Bomb, 5, 0, game->getPlayer2());
    auto scoutPiece2 = new Pieces(2, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(bombPiece2, game->getPlayer2());
    game->addPiece(scoutPiece2, game->getPlayer1());
    game->setPieceOnBoard(bombPiece2, 5, 0);
    game->setPieceOnBoard(scoutPiece2, 4, 0);

    engine->attack(scoutPiece2, {5, 0});
    BOOST_CHECK(game->getPlayer2()->getCaptured().size() == 1);
    BOOST_CHECK(game->getPlayer1()->getCaptured().size() == 2);

    game->removeFromBoard(scoutPiece2);
    game->removeFromBoard(bombPiece2);

    // Case 3: Attack Marshal with a Spy
    auto marshalPiece = new Pieces(10, PieceType::Marshal, 5, 0, game->getPlayer2());
    auto spyPiece = new Pieces(1, PieceType::Spy, 4, 0, game->getPlayer1());
    game->addPiece(marshalPiece, game->getPlayer2());
    game->addPiece(spyPiece, game->getPlayer1());
    game->setPieceOnBoard(marshalPiece, 5, 0);
    game->setPieceOnBoard(spyPiece, 4, 0);

    engine->attack(spyPiece, {5, 0});
    BOOST_CHECK(game->getPlayer1()->getCaptured().size() == 3);
    BOOST_CHECK(board->getPiece({5, 0}) == spyPiece);

    game->removeFromBoard(spyPiece);
    game->removeFromBoard(marshalPiece);

    // Case 4: Attack a weaker opponent
    auto weakOpponent = new Pieces(1, PieceType::Spy, 5, 0, game->getPlayer2());
    auto scoutPiece3 = new Pieces(2, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(weakOpponent, game->getPlayer2());
    game->addPiece(scoutPiece3, game->getPlayer1());
    game->setPieceOnBoard(weakOpponent, 5, 0);
    game->setPieceOnBoard(scoutPiece3, 4, 0);

    engine->attack(scoutPiece3, {5, 0});
    BOOST_CHECK(game->getPlayer1()->getCaptured().size() == 4);
    BOOST_CHECK(board->getPiece({5, 0}) == scoutPiece3);

    game->removeFromBoard(scoutPiece3);
    game->removeFromBoard(weakOpponent);

    // Case 5: Attack a stronger opponent
    auto strongOpponent = new Pieces(4, PieceType::Sergeant, 5, 0, game->getPlayer2());
    auto scoutPiece4 = new Pieces(2, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(strongOpponent, game->getPlayer2());
    game->addPiece(scoutPiece4, game->getPlayer1());
    game->setPieceOnBoard(strongOpponent, 5, 0);
    game->setPieceOnBoard(scoutPiece4, 4, 0);

    engine->attack(scoutPiece4, {5, 0});
    BOOST_CHECK(game->getPlayer2()->getCaptured().size() ==2);
    BOOST_CHECK(board->getPiece({5, 0}) == strongOpponent);

    game->removeFromBoard(scoutPiece4);
    game->removeFromBoard(strongOpponent);

    // Case 6: Tie (both pieces of the same value)
    auto tiedPiece = new Pieces(2, PieceType::Scout, 5, 0, game->getPlayer2());
    auto tiedAttacker = new Pieces(2, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(tiedPiece, game->getPlayer2());
    game->addPiece(tiedAttacker, game->getPlayer1());
    game->setPieceOnBoard(tiedPiece, 5, 0);
    game->setPieceOnBoard(tiedAttacker, 4, 0);

    engine->attack(tiedAttacker, {5, 0});
    BOOST_CHECK(board->getPiece({5, 0}) == nullptr);
    BOOST_CHECK(board->getPiece({4, 0}) == nullptr);
    BOOST_CHECK(game->getPlayer1()->getCaptured().size() == 5);
    BOOST_CHECK(game->getPlayer2()->getCaptured().size() == 3);

    game->removeFromBoard(tiedPiece);
    game->removeFromBoard(tiedAttacker);

    // Cleanup
    delete engine;
    delete game;
}



BOOST_AUTO_TEST_CASE(TestIsValidMove)
{
    // Initialize the game and engine
    auto game = new Game();
    auto engine = new engine::Engine(game);

    // Create a piece for the current player
    auto myPiece = new Pieces(10, PieceType::Marshal, 4, 0, game->getPlayer1());
    game->addPiece(myPiece, game->getPlayer1());
    game->setPieceOnBoard(myPiece, 4, 0);


    // Mock the possible positions for the piece

    // Case 1: Valid move
    BOOST_CHECK_MESSAGE(engine->isValidMove(myPiece, {4, 1}), "Move to (4, 1) should be valid.");
    BOOST_CHECK_MESSAGE(engine->isValidMove(myPiece, {5, 0}), "Move to (5, 0) should be valid.");

    // Case 2: Invalid move
    BOOST_CHECK_MESSAGE(!engine->isValidMove(myPiece, {6, 0}), "Move to (6, 0) should be invalid.");
    BOOST_CHECK_MESSAGE(!engine->isValidMove(myPiece, {0, 0}), "Move to (0, 0) should be invalid.");

    // Case 3: Edge case - Move outside the board boundaries
    BOOST_CHECK_MESSAGE(!engine->isValidMove(myPiece, {4, -2}), "Move to (4, -2) should be invalid (out of bounds).");

    // Case 4: Edge case - Same position
    BOOST_CHECK_MESSAGE(!engine->isValidMove(myPiece, {4, 0}), "Move to (4, 0) should be invalid (same position).");

    // Cleanup
    delete engine;
    delete game;
    delete myPiece;
}

BOOST_AUTO_TEST_CASE(TestCheckWin)
{
    // Create a game instance and an engine
    auto game = new Game();
    auto engine = new engine::Engine(game);

    // Case 1: Flag is captured
    auto flag = new Pieces(0, PieceType::Flag, 0, 0, game->getPlayer2());
    game->addCaptured(flag, game->getPlayer1());
    BOOST_CHECK(engine->checkWin() == engine::WinCondition::FlagCaptured);

    delete game;
    delete engine;

    // Reset the game state
    game = new Game();
    engine = new engine::Engine(game);
    game->setCurrentPlayer(game->getPlayer1());

    // Case 2: No valid moves for the opponent
    game->setCurrentPlayer(game->getPlayer2());
    BOOST_CHECK(!game->hasValidMoves());
    BOOST_CHECK(engine->checkWin() == engine::WinCondition::NoValidMoves);

    // Reset the game state
    game->setCurrentPlayer(game->getPlayer1());
    auto scout = new Pieces(2,Scout,1,1,game->getPlayer1());
    game->addPiece(scout,game->getPlayer1());
    game->setPieceOnBoard(scout,1,1);

    // Case 3: No win condition
    BOOST_CHECK(!game->isFlagCaptured());
    BOOST_CHECK(game->hasValidMoves());
    BOOST_CHECK(engine->checkWin() == engine::WinCondition::None);

    // Cleanup
    delete engine;
    delete game;
    delete flag;
}

BOOST_AUTO_TEST_SUITE_END()