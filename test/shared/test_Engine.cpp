//
// Created by matthieu on 20/12/24.
//

#define BOOST_TEST_MODULE BoardTest
#include <boost/test/included/unit_test.hpp>
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
    auto* previousPlayer = game->currentPlayer;
    engine->endTurn();
    auto newPlayer = game->currentPlayer;
    BOOST_CHECK(previousPlayer!=newPlayer);

    delete engine;
    delete game;
}

BOOST_AUTO_TEST_CASE(TestHandleCmdPlacement)
{
    auto game = new Game();
    auto engine = new engine::Engine(game);

    auto currentPlayer = game->currentPlayer;
    std::string filePath = "/home/matthieu/Cours/PLT/Stratego/src/shared/state/config/Balance.csv";
    engine->handleCmdPlacement(filePath);
    auto myPieces = currentPlayer->getMyPieces();
    auto nextPlayer = game->currentPlayer;

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

    game->currentPlayer = game->getPlayer1();

    // Create two pieces: one for the current player, one for the opponent
    auto myPiece = new Pieces(2, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(myPiece, game->getPlayer1());
    game->SetPieceOnBoard(myPiece, 4, 0);

    auto opponentPiece = new Pieces(1, PieceType::Spy, 5, 0, game->getPlayer2());
    game->addPiece(opponentPiece, game->getPlayer2());
    game->SetPieceOnBoard(opponentPiece, 5, 0);

    // Case 1: The piece does not belong to the current player
    BOOST_CHECK_MESSAGE(!engine->handleCmdMove({5, 0}, {5, 1}), "The piece should not belong to the current player.");
    game->currentPlayer = game->getPlayer1();

    // Case 2: The destination is invalid
    BOOST_CHECK_MESSAGE(!engine->handleCmdMove({4, 0}, {4, -1}), "The destination is out of bounds.");
    game->currentPlayer = game->getPlayer1();

    // Case 3: The destination is empty
    BOOST_CHECK_MESSAGE(engine->handleCmdMove({4, 0}, {4, 1}), "The move should be valid.");
    BOOST_CHECK_MESSAGE(board->getPiece({4, 1}) == myPiece, "The piece should have been moved.");

    // Reset the board state
    game->currentPlayer = game->getPlayer1();
    game->RemoveFromBoard(myPiece);
    game->SetPieceOnBoard(myPiece, 4, 0);

    // Case 4: The destination contains an opponent's piece -> Attack
    BOOST_CHECK_MESSAGE(engine->handleCmdMove({4, 0}, {5, 0}), "The attack should succeed.");
    BOOST_CHECK_MESSAGE(game->getPlayer1()->getCaptured().size() == 1, "The player should have captured one piece.");

    // Case 5: The destination contains an allied piece
    auto alliedPiece = new Pieces(3, PieceType::Scout, 4, 1, game->getPlayer1());
    game->addPiece(alliedPiece, game->getPlayer1());
    game->SetPieceOnBoard(alliedPiece, 4, 1);

    BOOST_CHECK_MESSAGE(!engine->handleCmdMove({4, 0}, {4, 1}), "A piece cannot move to a tile containing an allied piece.");

    // Cleanup for allied piece
    game->RemoveFromBoard(alliedPiece);
    delete alliedPiece;

    // Cleanup for game objects
    delete engine;
    delete game;
    delete myPiece;
    delete opponentPiece;
}

BOOST_AUTO_TEST_CASE(TestAttack)
{
    // Initialize the game and engine
    auto game = new Game();
    auto engine = new engine::Engine(game);
    auto board = game->getBoard();

    game->currentPlayer = game->getPlayer1();

    // Case 1: No target at the position
    auto myPiece = new Pieces(3, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(myPiece, game->getPlayer1());
    game->SetPieceOnBoard(myPiece, 4, 0);

    std::pair<int, int> emptyPosition = {5, 5};
    BOOST_CHECK_NO_THROW(engine->attack(myPiece, emptyPosition));
    BOOST_CHECK_MESSAGE(board->getPiece(emptyPosition) == nullptr, "No target should be found at the empty position.");

    // Case 2: Attack a Bomb with a Miner
    auto bombPiece = new Pieces(0, PieceType::Bomb, 5, 0, game->getPlayer2());
    game->addPiece(bombPiece, game->getPlayer2());
    game->SetPieceOnBoard(bombPiece, 5, 0);

    auto minerPiece = new Pieces(1, PieceType::Miner, 4, 0, game->getPlayer1());
    game->addPiece(minerPiece, game->getPlayer1());
    game->SetPieceOnBoard(minerPiece, 4, 0);

    engine->attack(minerPiece, {5, 0});
    BOOST_CHECK_MESSAGE(game->getPlayer1()->getCaptured().size() == 1, "Miner should capture the Bomb.");
    BOOST_CHECK_MESSAGE(board->getPiece({5, 0}) == minerPiece, "Miner should occupy the Bomb's position.");

    // Reset the board
    game->RemoveFromBoard(minerPiece);
    game->SetPieceOnBoard(myPiece, 4, 0);

    // Case 3: Attack Marshal with a Spy
    auto marshalPiece = new Pieces(10, PieceType::Marshal, 5, 0, game->getPlayer2());
    game->addPiece(marshalPiece, game->getPlayer2());
    game->SetPieceOnBoard(marshalPiece, 5, 0);

    auto spyPiece = new Pieces(1, PieceType::Spy, 4, 0, game->getPlayer1());
    game->addPiece(spyPiece, game->getPlayer1());
    game->SetPieceOnBoard(spyPiece, 4, 0);

    engine->attack(spyPiece, {5, 0});
    BOOST_CHECK_MESSAGE(game->getPlayer1()->getCaptured().size() == 2, "Spy should capture the Marshal.");
    BOOST_CHECK_MESSAGE(board->getPiece({5, 0}) == spyPiece, "Spy should occupy the Marshal's position.");

    // Reset the board
    game->RemoveFromBoard(spyPiece);
    game->SetPieceOnBoard(myPiece, 4, 0);

    // Case 4: Attack a weaker opponent
    auto weakOpponent = new Pieces(2, PieceType::Scout, 5, 0, game->getPlayer2());
    game->addPiece(weakOpponent, game->getPlayer2());
    game->SetPieceOnBoard(weakOpponent, 5, 0);

    engine->attack(myPiece, {5, 0});
    BOOST_CHECK_MESSAGE(game->getPlayer1()->getCaptured().size() == 3, "Player should capture the weaker opponent.");
    BOOST_CHECK_MESSAGE(board->getPiece({5, 0}) == myPiece, "Attacker should occupy the opponent's position.");

    // Reset the board
    game->RemoveFromBoard(myPiece);
    game->SetPieceOnBoard(myPiece, 4, 0);

    // Case 5: Attack a stronger opponent
    auto strongOpponent = new Pieces(4, PieceType::Sergeant, 5, 0, game->getPlayer2());
    game->addPiece(strongOpponent, game->getPlayer2());
    game->SetPieceOnBoard(strongOpponent, 5, 0);

    engine->attack(myPiece, {5, 0});
    BOOST_CHECK_MESSAGE(game->getPlayer2()->getCaptured().size() == 1, "Stronger opponent should capture the attacking piece.");
    BOOST_CHECK_MESSAGE(board->getPiece({5, 0}) == strongOpponent, "Opponent should remain at their position.");

    // Reset the board
    game->RemoveFromBoard(myPiece);

    // Case 6: Tie (both pieces of the same value)
    auto tiedPiece = new Pieces(3, PieceType::Scout, 5, 0, game->getPlayer2());
    game->addPiece(tiedPiece, game->getPlayer2());
    game->SetPieceOnBoard(tiedPiece, 5, 0);

    auto tiedAttacker = new Pieces(3, PieceType::Scout, 4, 0, game->getPlayer1());
    game->addPiece(tiedAttacker, game->getPlayer1());
    game->SetPieceOnBoard(tiedAttacker, 4, 0);

    engine->attack(tiedAttacker, {5, 0});
    BOOST_CHECK_MESSAGE(board->getPiece({5, 0}) == nullptr, "Both pieces should be removed after a tie.");
    BOOST_CHECK_MESSAGE(game->getPlayer1()->getCaptured().size() == 4, "Attacker should capture the opponent in a tie.");
    BOOST_CHECK_MESSAGE(game->getPlayer2()->getCaptured().size() == 2, "Opponent should capture the attacker in a tie.");

    // Cleanup
    delete engine;
    delete game;
    delete myPiece;
    delete minerPiece;
    delete marshalPiece;
    delete weakOpponent;
    delete strongOpponent;
    delete tiedPiece;
    delete tiedAttacker;
}

BOOST_AUTO_TEST_CASE(TestIsValidMove)
{
    // Initialize the game and engine
    auto game = new Game();
    auto engine = new engine::Engine(game);

    // Create a piece for the current player
    Pieces* myPiece = new Pieces(10, PieceType::Marshal, 4, 0, game->getPlayer1());
    game->addPiece(myPiece, game->getPlayer1());
    game->SetPieceOnBoard(myPiece, 4, 0);


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
    game->currentPlayer = game->getPlayer1();

    // Case 2: No valid moves for the opponent
    game->currentPlayer = game->getPlayer2();
    BOOST_CHECK(!game->hasValidMoves());
    BOOST_CHECK(engine->checkWin() == engine::WinCondition::NoValidMoves);

    // Reset the game state
    game->currentPlayer = game->getPlayer1();
    auto scout = new Pieces(2,Scout,1,1,game->getPlayer1());
    game->addPiece(scout,game->getPlayer1());
    game->SetPieceOnBoard(scout,1,1);

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