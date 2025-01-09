#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <state.h>

using namespace server;
using namespace state;
const std::string ACK_MESSAGE = "ACK";

std::string getProjectRootDirectory()
{
    std::string filePath = __FILE__;
    std::size_t pos = filePath.find("Stratego_Berdij_Charret_Preys_Pillot");
    if (pos == std::string::npos)
    {
        throw std::runtime_error("Unable to find 'Stratego_Berdij_Charret_Preys_Pillot' in the path");
    }

    return filePath.substr(0, pos + std::string("Stratego_Berdij_Charret_Preys_Pillot").length());
}


std::string constructPath(const std::string& relativePath)
{
    return getProjectRootDirectory() + "/" + relativePath;
}


Server::Server(int port, bool running,Game* game) : server_fd(-1), port(port), running(running), game(game) {}

Server::~Server() {
    stop();
}

void Server::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        throw std::runtime_error("Failed to create socket");
    }

    // Activer SO_REUSEADDR pour permettre la réutilisation du port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        throw std::runtime_error("Failed to set SO_REUSEADDR");
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

void Server::sendData(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.size(), 0);
    if (!waitForAcknowledgment(client_fd)) {
        throw std::runtime_error("Failed to receive acknowledgment sendLargeJson.");
    }
}

void Server::broadcastGameState() {
    auto gameState = serializeGameState();
    for (int client_fd : clients)
    {
        sendLargeJson(client_fd, gameState);
    }
}

void Server::sendRequestToClient(int client_fd, ServerRequest& request) {
    std::string serializedRequest = request.serialize();
    //std::cout<<"serializedRequest :"<<serializedRequest<<std::endl;
    ssize_t sent = send(client_fd, serializedRequest.c_str(), serializedRequest.size(), 0);
    if (sent != static_cast<ssize_t>(serializedRequest.size())) {
        perror("Failed to send request");
        throw std::runtime_error("Error sending request to client.");
    }
    std::cout << "Sending request to client " << client_fd << ": " << serializedRequest << std::endl;
    if (!waitForAcknowledgment(client_fd)) {
        throw std::runtime_error("Failed to receive acknowledgment sendLargeJson.");
    }
    std::cout << "ACK received" << std::endl;
}

Json::Value Server::receiveResponseFromClient(int clientId) {
    std::cout<<"Waiting for response"<<std::endl;
    char buffer[4096];
    ssize_t bytesReceived = recv(clientId, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        throw std::runtime_error("Failed to receive response from client.");
    }
    std::cout<<"Response received"<<std::endl;
    buffer[bytesReceived] = '\0';
    std::string jsonString(buffer);
    std::istringstream jsonStream(jsonString);
    //std::cout<<"jsonString :"<<jsonString<<std::endl;
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::string errors;

    if (!Json::parseFromStream(reader, jsonStream, &root, &errors)) {
        throw std::runtime_error("Failed to parse JSON response: " + errors);
    }
    try {
        sendAcknowledgment(clientId);
    } catch (const std::exception& e) {
        std::cerr << "Failed to send acknowledgment receiveRequest: " << e.what() << std::endl;
        throw;
    }
    return root;
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
    for (int i=0; i<clients.size(); i++) {
        // Envoie de l'identifiant au client
        ssize_t sent = send(clients[i], &i, sizeof(i), 0);
        if (sent != sizeof(i)) {
            perror("Failed to send identifier");
            throw std::runtime_error("Error sending identifier to client.");
        }
        std::cout << "Sent identifier " << i << " to client " <<clients[i] << std::endl;
        if (!waitForAcknowledgment(clients[i])) {
            throw std::runtime_error("Failed to receive acknowledgment sendIdentifier.");
        }
        std::cout<<"ACK received"<<std::endl;
    }
}

std::vector<int> Server::handleClientResponse(int client_fd, Json::Value& response) {
    RequestType type = static_cast<RequestType>(response["type"].asInt());
    const Json::Value& data = response["response"];
    std::vector<int> coords;

    switch (type) {
    case RequestType::Move:
        std::cout << "Client Move Response: from (" << data["fromX"].asInt() << ", " << data["fromY"].asInt()
                  << ") to (" << data["toX"].asInt() << ", " << data["toY"].asInt() << ")" << std::endl;
        coords.push_back(data["fromY"].asInt());
        coords.push_back(data["fromX"].asInt());
        coords.push_back(data["toY"].asInt());
        coords.push_back(data["toX"].asInt());
        return coords;

    case RequestType::Configuration:
        std::cout << "Client Configuration Response:  " << data["choice"].asInt() << std::endl;
        coords.push_back(data["choice"].asInt());
        return coords;

    default:
        std::cerr << "Unknown response type received." << std::endl;
        break;
    }
}


bool server::Server::waitForAcknowledgment(int client_fd) {
    std::cout<<"Waiting for ACK"<<std::endl;
    char buffer[4096];
    ssize_t bytesReceived = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    /*if (bytesReceived <= 0) {
        throw std::runtime_error("Failed to receive acknowledgment from client.");
    }*/

    buffer[bytesReceived] = '\0';
    std::string response(buffer);

    return response == ACK_MESSAGE; // Retourne true si l'acquittement est reçu
}


void server::Server::sendAcknowledgment(int client_fd) {
    std::cout<<"sending ACK :"<<ACK_MESSAGE<<std::endl;
    ssize_t sent = send(client_fd, ACK_MESSAGE.c_str(), ACK_MESSAGE.size(), 0);

    if (sent != static_cast<ssize_t>(ACK_MESSAGE.size())) {
        throw std::runtime_error("Failed to send acknowledgment to client.");
    }
    std::cout<<"ACK sent :"<<ACK_MESSAGE<<std::endl;
}

void Server::sendLargeJson(int client_fd, const std::string& jsonString) {
    // Envoyez la taille totale du fichier JSON
    uint32_t totalSize = htonl(jsonString.size());
    ssize_t sent = send(client_fd, &totalSize, sizeof(totalSize), 0);
    if (sent != sizeof(totalSize)) {
        throw std::runtime_error("Failed to send total size.");
    }

    // Envoyez les données en chunks
    size_t chunkSize = 4096; // Taille du chunk (4 KB)
    size_t bytesSent = 0;

    while (bytesSent < jsonString.size()) {
        size_t remaining = jsonString.size() - bytesSent;
        size_t currentChunkSize = std::min(chunkSize, remaining);

        sent = send(client_fd, jsonString.data() + bytesSent, currentChunkSize, 0);
        if (sent != static_cast<ssize_t>(currentChunkSize)) {
            throw std::runtime_error("Failed to send JSON chunk.");
        }

        bytesSent += sent;

        // Attendez un acquittement pour chaque chunk
        /*if (!waitForAcknowledgment(client_fd)) {
            throw std::runtime_error("Failed to receive acknowledgment sendLargeJson.");
        }*/
    }
    if (!waitForAcknowledgment(client_fd)) {
        throw std::runtime_error("Failed to receive acknowledgment sendLargeJson.");
    }
    std::cout << "Ack received for largeJson" << std::endl;
}


void Server::handlePlacement(engine::Engine* gameEngine)
{
    ServerRequest configRequest;
    configRequest.type = server::RequestType::Configuration;
    for (int client_fd : clients)
    {
        sendRequestToClient(client_fd, configRequest);
        Json::Value configResponse = receiveResponseFromClient(client_fd);
        auto choice = handleClientResponse(client_fd, configResponse)[0];
        std::string filepath;
        switch (choice)
        {
        case 1:
            filepath = constructPath("src/shared/state/config/Offensive.csv");
            break;
        case 2:
            filepath = constructPath("src/shared/state/config/Defensive.csv");
            break;
        case 3:
            filepath = constructPath("src/shared/state/config/Balance.csv");
            break;
        default:
            filepath = constructPath("src/shared/state/config/Defensive.csv");
        }
        gameEngine->handleCmdPlacement(filepath);
    }
}


