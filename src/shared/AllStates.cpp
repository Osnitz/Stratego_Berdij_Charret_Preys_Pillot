#include <iostream>
#include <random>

#include "state.h"
#include "engine.h"

using namespace state;
using namespace engine;
Board* board = Board::getInstance();

void InitState::enter(Game* game)
{
    printf("---InitState ---\n");
    printf("Welcome to the game!\n");
    printf("Do you want to play against the AI or another Player? (1/2): \n");
    std::string answer;
    std::cin >> answer;
    if (answer == "1")
    {
        game->setAI(true);
    }
    else
    {
        game->setAI(false);
    }
    handleInput(game);
}

void InitState::handleInput(Game* game)
{
    update(game);
}

void InitState::update(Game* game)
{
    game->setState(new PlacementState());
}


void PlacementState::enter(Game* game)
{
    printf("--- PlacementState---\n");
    game->switchTurn();
    if (game->againstIA)
    {
        game->setState(new AIPlacementState());
    }
    else
    {
        handleInput(game);
    }
}

void PlacementState::handleInput(Game* game)
{
    for (int i = 0; i < 2; ++i)
    {
        auto player = game->getCurrentPlayer();
        loadPlayerConfig(player);
        game->switchTurn();
    }
    update(game);
}

void PlacementState::loadPlayerConfig(Player* player)
{
    std::cout << "Quelle configuration voulez-vous ? \n";
    std::cout << "1 : Offensive, 2 : Defensive, 3 : Balance \n";
    std::string choice;
    std::cin >> choice;
    if (choice == "1")
    {
        player->loadConfig(TODO);
    }
    else if (choice == "2")
    {
        player->loadConfig(TODO);
    }
    else if (choice == "3")
    {
        player->loadConfig(TODO);
    }
    else if (choice == "7")
    {
        player->loadConfig(TODO);
    }
    else
    {
        std::cerr << "Incorrect choice entered" << std::endl;
        loadPlayerConfig(player);
    }
}

void PlacementState::update(Game* game)
{
    if (game->getCurrentPlayer() == game->getPlayer2() && game->againstIA)
    {
        game->setState(new AITurnState());
    }
    else
    {
        game->setState(new PlayerTurnState());
    }
}


void AIPlacementState::enter(Game* game)
{
    handleInput(game);
}

void AIPlacementState::handleInput(Game* game)
{
    auto player = game->getCurrentPlayer();

    if (game->getPlayer1() == player)
    {
        // Load configuration for Player 1
        std::cout << "Player 1, choose your configuration: \n";
        std::cout << "1: Offensive, 2: Defensive, 3: Balance \n";
        std::string choice1;
        while (true)
        {
            std::cin >> choice1;
            if (choice1 == "1" || choice1 == "2" || choice1 == "3" || choice1 == "7")
            {
                break;
            }
            std::cerr << "Incorrect choice entered" << std::endl;
        }
        if (choice1 == "1")
        {
            player->loadConfig(TODO);
        }
        else if (choice1 == "2")
        {
            player->loadConfig(TODO);
        }
        else if (choice1 == "3")
        {
            player->loadConfig(TODO);
        }
        else if (choice1 == "7")
        {
            player->loadConfig(TODO);
        }
        game->switchTurn();
        player = game->getCurrentPlayer();
        // Load configuration for Player 2 (AI)
        std::cout << "Loading AI configuration...\n" << std::endl;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 3);
        std::string choice2 = std::to_string(distr(gen));
        if (choice2 == "1")
        {
            player->loadConfig(TODO);
        }
        else if (choice2 == "2")
        {
            player->loadConfig(TODO);
        }
        else if (choice2 == "3")
        {
            player->loadConfig(TODO);
        }
        else if (choice2 == "7")
        {
            player->loadConfig(TODO);
        }
        game->switchTurn();
        update(game);
        return;
    }

    player = game->getCurrentPlayer();
    // Load configuration for Player 2 (AI)
    std::cout << "Loading AI configuration...\n" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 3);
    std::string choice2 = std::to_string(distr(gen));
    if (choice2 == "1")
    {
        player->loadConfig(TODO);
    }
    else if (choice2 == "2")
    {
        player->loadConfig(TODO);
    }
    else if (choice2 == "3")
    {
        player->loadConfig(TODO);
    }
    else if (choice2 == "7")
    {
        player->loadConfig(TODO);
    }

    game->switchTurn();
    player = game->getCurrentPlayer();
    std::cout << "Player 1, choose your configuration: \n";
    std::cout << "1: Offensive, 2: Defensive, 3: Balance \n";
    std::string choice1;
    while (true)
    {
        std::cin >> choice1;
        if (choice1 == "1" || choice1 == "2" || choice1 == "3")
        {
            break;
        }
        std::cerr << "Incorrect choice entered" << std::endl;
    }

    if (choice1 == "1")
    {
        player->loadConfig(TODO);
    }
    else if (choice1 == "2")
    {
        player->loadConfig(TODO);
    }
    else if (choice1 == "3")
    {
        player->loadConfig(TODO);
    }
    else if (choice1 == "7")
    {
        player->loadConfig(TODO);
    }
    game->switchTurn();
    update(game);
}

void AIPlacementState::update(Game* game)
{
    game->setState(new PlayerTurnState());
}


void PlayerTurnState::enter(Game* game)
{
    printf("--- PlayerTurnState ---\n");
    std::cout << "It's time for : " << (game->getCurrentPlayer() == game->getPlayer1() ? "Player 1" : "Player 2\n") <<
        std::endl;
    board->displayBoard(*game->getCurrentPlayer());
    handleInput(game);
}

void PlayerTurnState::handleInput(Game* game)
{
    if (game->Purgatory != nullptr)
    {
        game->getCurrentPlayer()->addCaptured(game->Purgatory);
        game->Purgatory = nullptr;
    }
    if (game->Graveyard != nullptr)
    {
        game->getCurrentPlayer()->removePiece(game->Graveyard);
        game->Graveyard = nullptr;
    }
    int x, y;

    std::cout << "Which piece do you want to play ? (format: x y)" << std::endl;
    while (true)
    {
        std::cin >> x >> y;
        if (std::cin.fail())
        {
            std::cerr << "Please enter an integer" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else { break; }
    }


    std::pair<int, int> position;
    position.first = x;
    position.second = y;


    Pieces* pieceToMove = board->getPiece(position);
    if (pieceToMove == nullptr)
    {
        std::cerr << "There is no piece in this location !" << std::endl;
        handleInput(game);
        return;
    }
    if (!game->getCurrentPlayer()->belongTo(pieceToMove))
    {
        std::cerr << "This is not your piece !" << std::endl;
        handleInput(game);
        return;
    }

    auto possiblePositions = pieceToMove->canMove(pieceToMove);
    if (possiblePositions.empty())
    {
        std::cerr << "This piece has no possible move !\n" << std::endl;
        handleInput(game);
        return;
    }

    std::cout << "What is your destination ? (format: x y)" << std::endl;
    int newx;
    int newy;
    while (true)
    {
        std::cin >> newx >> newy;
        if (std::cin.fail())
        {
            std::cerr << "Please enter an integer" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else { break; }
    }
    std::pair<int, int> destination;
    destination.first = newx;
    destination.second = newy;

    for (auto possible_position : possiblePositions)
    {
        if (possible_position == destination)
        {
            if (pieceToMove->CheckCase(destination) == "Enemy")
            {
                pieceToMove->attack(destination);
                update(game);
                return;
            }
            pieceToMove->setPosition(destination);
            update(game);
            return;
        }
    }
    std::cerr << "Invalid destination" << std::endl;
    pieceToMove->CheckBoard(destination, false);
    handleInput(game);
}

void PlayerTurnState::update(Game* game)
{
    Player* player = game->getCurrentPlayer();
    std::vector<Pieces*> capturedPieces = player->getCaptured();
    if (!capturedPieces.empty() && capturedPieces[0]->getValue() == 0)
    {
        game->setState(new WinState());
    }
    else
    {
        game->switchTurn();
        if (game->getCurrentPlayer() == game->getPlayer2() && game->againstIA)
        {
            game->setState(new AITurnState());
        }
        else
        {
            game->setState(new PlayerTurnState());
        }
    }
}


void AITurnState::enter(Game* game)
{
    printf("--- IATurnState ---\n");
    handleInput(game);
}

void AITurnState::handleInput(Game* game)
{
    Player* aiPlayer = game->getCurrentPlayer();
    std::vector<Pieces*> aiPieces = aiPlayer->getMyPieces();

    if (aiPieces.empty())
    {
        std::cerr << "No pieces available for AI to move." << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    // Filter the pieces with possible moves
    std::vector<Pieces*> movablePieces;
    for (auto* piece : aiPieces)
    {
        if (!piece->canMove(piece).empty())
        {
            movablePieces.push_back(piece);
        }
    }

    if (movablePieces.empty())
    {
        std::cerr << "No valid moves available for AI." << std::endl;
        return;
    }

    // Select a piece at random from those that can move
    std::uniform_int_distribution<> distr(0, movablePieces.size() - 1);
    Pieces* pieceToMove = movablePieces[distr(gen)];

    // Get the possible positions for this piece
    auto possiblePositions = pieceToMove->canMove(pieceToMove);
    std::uniform_int_distribution<> distr2(0, possiblePositions.size() - 1);
    auto destination = possiblePositions[distr2(gen)];

    if (pieceToMove->CheckCase(destination) == "Enemy")
    {
        pieceToMove->attack(destination);
        update(game);
        return;
    }
    pieceToMove->setPosition(destination);
    update(game);
}

void AITurnState::update(Game* game)
{
    game->switchTurn();
    game->setState(new PlayerTurnState());
}


void WinState::enter(Game* game)
{
    printf("The flag has been captured! Congratulations! \n YOU WIN!!!!\n");
    handleInput(game);
    //game->endGame();
}

void WinState::handleInput(Game* game)
{
    update(game);
}

void WinState::update(Game* game)
{
    game->setState(new WaitingState());
}


void WaitingState::enter(Game* game)
{
    printf("--- WaitingState ---\n");
    handleInput(game);
}

void WaitingState::handleInput(Game* game)
{
    printf("Do you want to play again? (y/n): \n");
    char answer;
    std::cin >> answer;
    if (answer == 'y')
    {
        update(game);
    }
    else
    {
        std::cout << "\nThank you for playing! See you next time.\n" << std::endl;
        exit(0);
    }
}

void WaitingState::update(Game* game)
{
    game->setState(new InitState());
}


State::~State()
{
}
