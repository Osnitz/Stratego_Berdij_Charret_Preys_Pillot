#include "client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <json/json.h>

using namespace client;
using namespace state;

NetworkClient::NetworkClient(std::string server_ip, int port,Game* game) : client_fd(-1), server_ip(server_ip),
server_port(port), game(game)
{
}

NetworkClient::~NetworkClient()
{
    disconnect();
}

void NetworkClient::connectToServer()
{
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0)
    {
        perror("Invalid address/Address not supported");
        return;
    }

    if (connect(client_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection failed");
        return;
    }

    std::cout << "Connected to server." << std::endl;
}

void NetworkClient::disconnect()
{
    if (client_fd != -1)
    {
        close(client_fd);
        client_fd = -1;
    }
}

void NetworkClient::sendData(const std::string& message)
{
    send(client_fd, message.c_str(), message.size(), 0);
}

std::string NetworkClient::receiveData()
{
    char buffer[1024] = {0};
    ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received < 0)
    {
        perror("Receive failed");
        return "";
    }
    return std::string(buffer, bytes_received);
}

void NetworkClient::handleServerRequest()
{
    std::string request = receiveData(); // Lire la requête
    if (!request.empty())
    {
        std::cout << "Server: " << request << std::endl;

        // Simuler un input
        /*std::string response;
        std::cout << "Enter your response: ";
        std::getline(std::cin, response);

        sendData(response); // Envoyer la réponse*/
    }
}

void NetworkClient::monitorServerConnection()
{
    char buffer[1]; // Buffer minimal pour tester la connexion

    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), MSG_PEEK);
    if (bytes_received == 0)
    {
        // Le serveur s'est déconnecté proprement
        std::cout << "Server disconnected. Closing client..." << std::endl;
        disconnect();
        exit(0); // Quitte le programme ou gère la fin de connexion
    }
    else if (bytes_received < 0)
    {
        perror("Error while monitoring server connection");
    }

    // Petite pause pour éviter une surcharge du CPU
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

GameState NetworkClient::deserializeGameState(const std::string& jsonString)
{
    // Parse le JSON reçu
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::string errors;

    std::istringstream jsonStream(jsonString);
    if (!Json::parseFromStream(reader, jsonStream, &root, &errors)) {
        throw std::runtime_error("Failed to parse JSON: " + errors);
    }


    // Préparez le tableau 1D pour les pièces
    GameState gameState;

    // Vérifiez si la clé "pieces" existe
    if (!root.isMember("pieces") || !root["pieces"].isArray()) {
        throw std::runtime_error("Invalid JSON: Missing 'pieces' array.");
    }

    // Désérialisez chaque pièce
    for (const auto& jsonPiece : root["pieces"]) {
        int value = jsonPiece["value"].asInt();
        int x = jsonPiece["x"].asInt();
        int y = jsonPiece["y"].asInt();
        int ownerID = jsonPiece["owner"].asInt();
        Player* owner;
        switch (ownerID)
        {
        case 0:
            owner = game->getPlayer1();
            break;
        case 1:
            owner = game->getPlayer2();
            break;
        default:
            throw std::runtime_error("Invalid owner ID in JSON.");
        }
            // Implémentez cette méthode pour associer l'ID au Player*
        PieceType type = static_cast<PieceType>(jsonPiece["type"].asInt());
        bool revealed = jsonPiece["revealed"].asBool();

        // Créez la pièce et ajoutez-la au tableau
        auto* piece = new Pieces(value, type,x, y, owner);
        piece->setReveal(revealed);
        gameState.pieces.push_back(piece);
        game->addPiece(piece,owner);
    }
    gameState.currentPlayerID = root["currentPlayer"].asInt();
    return gameState;
}

void NetworkClient::updateGameState(GameState gameState)
{
    // Met à jour le jeu avec les nouvelles pièces
    game->clearBoard();
    for (Pieces* piece : gameState.pieces)
    {
        game->setPieceOnBoard(piece, piece->getPosition().first, piece->getPosition().second);
    }
    game->setCurrentPlayer(game->getPlayerByID(gameState.currentPlayerID));
}

std::string NetworkClient::receiveGameStateWithDynamicBuffer() {
    std::string jsonString;
    char buffer[4096];
    ssize_t bytesReceived;

    while ((bytesReceived = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        jsonString.append(buffer, bytesReceived);

        // Vérifiez si le JSON est complet (par exemple, dernier caractère = '}')
        if (jsonString.back() == '}') {
            break;
        }
    }

    if (bytesReceived < 0) {
        throw std::runtime_error("Failed to receive JSON data.");
    }

    return jsonString;
}


int NetworkClient::receiveIdentifier() {
    int identifier;
    ssize_t received = recv(client_fd, &identifier, sizeof(identifier), 0);
    if (received != sizeof(identifier)) {
        perror("Failed to receive identifier");
        throw std::runtime_error("Error receiving identifier from server.");
    }
    std::cout << "Received identifier: " << identifier << std::endl;
    return identifier;
}

int NetworkClient::getPlayerID() {
    return playerID;
}

void NetworkClient::setPlayerId(int playerId) {
    playerID = playerId;
}