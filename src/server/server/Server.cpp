#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <state.h>

using namespace server;
using namespace state;

Server::Server(int port, bool running,Game* game) : server_fd(-1), port(port), running(running), game(game) {}

Server::~Server() {
    stop();
}

void Server::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return;
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return;
    }

    running = true;
    std::cout << "Server is listening on port " << port << "..." << std::endl;
}

void Server::stop() {
    running = false;
    for (int client_fd : clients) {
        close(client_fd);
    }
    clients.clear();
    if (server_fd != -1) {
        close(server_fd);
        server_fd = -1;
    }
    std::cout << "Server stopped." << std::endl;
}

void Server::acceptClients() {
    sockaddr_in client_address{};
    socklen_t client_len = sizeof(client_address);

    int client_fd = accept(server_fd, (sockaddr*)&client_address, &client_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return;
    }

    clients.push_back(client_fd);
    clientIds[client_fd] = nextClientId++;
    std::cout << "New client connected with ID: " << clientIds[client_fd] << std::endl;
}


std::string Server::receiveData(int client_fd) {
    char buffer[1024] = {0};
    ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received < 0) {
        perror("Receive failed");
        return "";
    }
    return std::string(buffer, bytes_received);
}

void Server::sendData(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.size(), 0);
}

void Server::broadcastData(const std::string& message) {
    for (int client_fd : clients) {
        sendData(client_fd, message);
    }
}

void Server::sendRequestToClient(int clientId, const std::string& request) {
    for (const auto& [fd, id] : clientIds) {
        if (id == clientId) {
            sendData(fd, request); // Méthode existante
            std::cout << "Request sent to client " << clientId << ": " << request << std::endl;
            return;
        }
    }
    std::cerr << "Client " << clientId << " not found!" << std::endl;
}

std::string Server::waitForResponseFromClient(int clientId) {
    for (const auto& [fd, id] : clientIds) {
        if (id == clientId) {
            std::string response = receiveData(fd); // Méthode existante
            //std::cout << "Response from client " << clientId << ": " << response << std::endl;
            return response;
        }
    }
    std::cerr << "Client " << clientId << " not found!" << std::endl;
    return "";
}

Json::Value Server::serializeOnePiece(Pieces* piece) {
    Json::Value jsonPiece;
    jsonPiece["value"] = piece->getValue();
    jsonPiece["x"] = piece->getPosition().first;
    jsonPiece["y"] = piece->getPosition().second;
    jsonPiece["range"] = piece->getRange();
    jsonPiece["owner"] = piece->getOwner()->getPlayerID();
    jsonPiece["type"] = piece->getType();
    jsonPiece["revealed"] = piece->isRevealed();
    return jsonPiece;
}

std::string Server::serializeGameState() {
    Json::Value root;
    auto board = game->getBoard();
    auto grid = *board->getGrid();
    for (auto row:grid)
    {
        for (auto piecePtr:row)
        {
            if (piecePtr ==nullptr)
            {
                continue;
            }
            root["pieces"].append(serializeOnePiece(piecePtr));
        }
    }
    root["currentPlayer"] = game->getCurrentPlayer()->getPlayerID();
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

void Server::sendIdentifierToClients() {
    for (const auto& [client_fd, clientId] : clientIds) {
        // Envoie de l'identifiant au client
        ssize_t sent = send(client_fd, &clientId, sizeof(clientId), 0);
        if (sent != sizeof(clientId)) {
            perror("Failed to send identifier");
            throw std::runtime_error("Error sending identifier to client.");
        }
        std::cout << "Sent identifier " << clientId << " to client " << client_fd << std::endl;
    }
}

