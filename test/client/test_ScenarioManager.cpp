//
// Created by matthieu on 22/12/24.
//

#include <ai/HeuristicAI.h>
#include <ai/RandomAI.h>
#include <boost/test/included/unit_test.hpp>
#include "state.h"
#include "client.h"

using namespace state;
using namespace client;


BOOST_AUTO_TEST_SUITE(ScenarioManagerTestSuite)
    BOOST_AUTO_TEST_CASE(TestScenarioManagerConstructor)
    {
        auto engine = new engine::Engine(new Game());
        auto scenarioManager = new ScenarioManager(engine);
        // Verify that the engine pointer is correctly set
        BOOST_CHECK(scenarioManager->getEngine() == engine);
        // Verify that the mode is set to the default (GameMode::PVP)
        BOOST_CHECK(scenarioManager->getMode() == GameMode::PVP);
        delete scenarioManager;
        delete engine;
    }

    BOOST_AUTO_TEST_CASE(TestScenarioManagerSetMode)
    {
        auto engine = new engine::Engine(new Game());
        auto scenarioManager = new client::ScenarioManager(engine);
        // Test setting mode to PVP
        scenarioManager->setMode(GameMode::PVP);
        BOOST_CHECK(scenarioManager->getMode() == GameMode::PVP);
        // Test setting mode to PVE
        scenarioManager->setMode(GameMode::PVE);
        BOOST_CHECK(scenarioManager->getMode() == GameMode::PVE);
        // Test setting mode to AIvsAI
        scenarioManager->setMode(GameMode::AIvsAI);
        BOOST_CHECK(scenarioManager->getMode() == GameMode::AIvsAI);
        delete scenarioManager;
        delete engine;
    }

    BOOST_AUTO_TEST_CASE(TestInitializeControllers)
    {
        auto engine = new engine::Engine(new Game());
        auto scenarioManager = new client::ScenarioManager(engine);
        // Case 1: PVP mode (both players are HumanControllers)
        scenarioManager->setMode(GameMode::PVP);
        scenarioManager->initializeControllers();
        auto controllers = *scenarioManager->getPlayerControllersVector();
        BOOST_CHECK(dynamic_cast<client::HumanController*>(controllers[0]) != nullptr);
        BOOST_CHECK(dynamic_cast<client::HumanController*>(controllers[1]) != nullptr);
        BOOST_CHECK(controllers.size() == 2);
        delete scenarioManager;
        scenarioManager = new ScenarioManager(engine);
        // Case 2: PVE mode (Player 1 is HumanController, Player 2 is AIController)
        scenarioManager->setMode(GameMode::PVE);
        scenarioManager->initializeControllers();
        controllers = *scenarioManager->getPlayerControllersVector();
        BOOST_CHECK(dynamic_cast<client::HumanController*>(controllers[0]) != nullptr);
        BOOST_CHECK(dynamic_cast<client::AIController*>(controllers[1]) != nullptr);
        BOOST_CHECK(controllers.size() == 2);
        delete scenarioManager;
        scenarioManager = new ScenarioManager(engine);
        // Case 3: AIvsAI mode (both players are AIControllers)
        scenarioManager->setMode(GameMode::AIvsAI);
        scenarioManager->initializeControllers();
        controllers = *scenarioManager->getPlayerControllersVector();
        BOOST_CHECK(dynamic_cast<client::AIController*>(controllers[0]) != nullptr);
        BOOST_CHECK(dynamic_cast<client::AIController*>(controllers[1]) != nullptr);
        BOOST_CHECK(controllers.size() == 2);
        delete scenarioManager;
        delete engine;
    }

    BOOST_AUTO_TEST_CASE(TestGetPlayerController)
    {
        auto engine = new engine::Engine(new Game());
        auto scenarioManager = new client::ScenarioManager(engine);
        auto player1 = engine->getGame()->getPlayer1();
        auto player2 = engine->getGame()->getPlayer2();
        scenarioManager->setMode(GameMode::PVP);
        scenarioManager->initializeControllers();
        // Case 1: Get controller for Player 1
        auto player1Controller = scenarioManager->getPlayerController(player1);
        auto controllers = *scenarioManager->getPlayerControllersVector();
        BOOST_CHECK(player1Controller == controllers[0]);
        BOOST_CHECK(dynamic_cast<client::HumanController*>(player1Controller) != nullptr);
        // Case 2: Get controller for Player 2
        auto player2Controller = scenarioManager->getPlayerController(player2);
        BOOST_CHECK(player2Controller == controllers[1]);
        BOOST_CHECK(dynamic_cast<client::HumanController*>(player2Controller) != nullptr);
        delete scenarioManager;
        delete engine;
    }

    BOOST_AUTO_TEST_CASE(TestGetScenarioChoice)
    {
        // Save the original buffer of std::cin
        std::streambuf* originalCinBuffer = std::cin.rdbuf();
        // Mock engine instance
        auto engine = new engine::Engine(new Game());
        auto scenarioManager = new client::ScenarioManager(engine);
        // Case 1: User chooses PvP (1)
        {
            std::istringstream input("0\n5\n1\n"); // invalid input first to test the while loop
            std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
            GameMode mode = scenarioManager->getScenarioChoice();
            BOOST_CHECK(mode == GameMode::PVP);
        }
        // Case 2: User chooses PvE (2) with AI difficulty 1
        {
            std::istringstream input("2\n5\n1\n");
            std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
            GameMode mode = scenarioManager->getScenarioChoice();
            BOOST_CHECK(mode == GameMode::PVE);
            BOOST_CHECK(dynamic_cast<ai::RandomAI*>(scenarioManager->getAiModule0()) != nullptr);
        }
        // Case 2bis: User chooses PvE (2) with AI difficulty 2
        {
            std::istringstream input("2\n2\n");
            std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
            GameMode mode = scenarioManager->getScenarioChoice();
            BOOST_CHECK(mode == GameMode::PVE);
            BOOST_CHECK(dynamic_cast<ai::HeuristicAI*>(scenarioManager->getAiModule0()) != nullptr);
        }
        // Case 2ter: User chooses PvE (2) with AI difficulty 3
        {
            std::istringstream input("2\n3\n");
            std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
            GameMode mode = scenarioManager->getScenarioChoice();
            BOOST_CHECK(mode == GameMode::PVE);
            BOOST_CHECK(dynamic_cast<ai::RandomAI*>(scenarioManager->getAiModule0()) != nullptr);
        }
        // Case 3: User chooses AIvsAI (3) with AI difficulties 1 and 1
        {
            std::istringstream input("3\n5\n1\n5\n1\n");// 5\n is invalid input first to test the while loop in both selection of AIs
            std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
            GameMode mode = scenarioManager->getScenarioChoice();
            BOOST_CHECK(mode == GameMode::AIvsAI);
            BOOST_CHECK(dynamic_cast<ai::RandomAI*>(scenarioManager->getAiModule0()) != nullptr);
            BOOST_CHECK(dynamic_cast<ai::RandomAI*>(scenarioManager->getAiModule1()) != nullptr);
        }

    // Case 3bis: User chooses AIvsAI (3) with AI difficulties 2 and 2
    {
            std::istringstream input("3\n2\n2\n");
            std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
            GameMode mode = scenarioManager->getScenarioChoice();
            BOOST_CHECK(mode == GameMode::AIvsAI);
            BOOST_CHECK(dynamic_cast<ai::HeuristicAI*>(scenarioManager->getAiModule0()) != nullptr);
            BOOST_CHECK(dynamic_cast<ai::HeuristicAI*>(scenarioManager->getAiModule1()) != nullptr);
    }
    // Case 3ter: User chooses AIvsAI (3) with AI difficulties 3 and 3
    {
            std::istringstream input("3\n3\n3\n");
            std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to use input stream
            GameMode mode = scenarioManager->getScenarioChoice();
            BOOST_CHECK(mode == GameMode::AIvsAI);
            BOOST_CHECK(dynamic_cast<ai::RandomAI*>(scenarioManager->getAiModule0()) != nullptr);
            BOOST_CHECK(dynamic_cast<ai::RandomAI*>(scenarioManager->getAiModule1()) != nullptr);
    }


        // Restore the original buffer of std::cin
        std::cin.rdbuf(originalCinBuffer);
        // Cleanup
        delete scenarioManager;
        delete engine;
    }

    BOOST_AUTO_TEST_CASE(TestGetEngine)
    {
        auto engine = new engine::Engine(new Game());
        auto scenarioManager = new client::ScenarioManager(engine);
        // Verify that getEngine() correctly returns the engine pointer
        BOOST_CHECK(scenarioManager->getEngine() == engine);
        delete scenarioManager;
        delete engine;
    }

    BOOST_AUTO_TEST_CASE(TestGetMode)
    {
        auto engine = new engine::Engine(new Game());
        auto scenarioManager = new client::ScenarioManager(engine);
        BOOST_CHECK(scenarioManager->getMode() == GameMode::PVP);
        scenarioManager->setMode(GameMode::PVE);
        BOOST_CHECK(scenarioManager->getMode() == GameMode::PVE);
        scenarioManager->setMode(GameMode::AIvsAI);
        BOOST_CHECK(scenarioManager->getMode() == GameMode::AIvsAI);
        delete scenarioManager;
        delete engine;
    }

BOOST_AUTO_TEST_SUITE_END()
