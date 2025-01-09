#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <server.h>

using namespace server;
using namespace state;
const std::string ACK_MESSAGE = "ACK";
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

void NetworkClient::handleServerRequest(ServerRequest& request)
{
    switch (request.type) {
    case RequestType::Move: {
            std::cout << "Move Request Received. Provide the coordinates:" << std::endl;

            int fromX, fromY, toX, toY;
            std::cout << "Enter fromX: ";
            std::cin >> fromX;
            std::cout << "Enter fromY: ";
            std::cin >> fromY;
            std::cout << "Enter toX: ";
            std::cin >> toX;
            std::cout << "Enter toY: ";
            std::cin >> toY;

            // Préparez la réponse
            Json::Value response;
            response["fromX"] = fromX;
            response["fromY"] = fromY;
            response["toX"] = toX;
            response["toY"] = toY;

            sendResponseToServer(request.type, response);
            break;
    }

    case RequestType::Configuration: {
            std::cout << "Configuration Request Received. Choose : (1) Offensive (2) Defensive, (3) Balance)\n" << std::endl;

            int choice;
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            // Préparez la réponse
            Json::Value response;
            response["choice"] = choice;

            sendResponseToServer(request.type, response);
            break;
    }

    default:
        std::cerr << "Unknown request type received." << std::endl;
        break;
    }
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
    game->clearBoard();
    for (Pieces* piece : gameState.pieces)
    {
        game->setPieceOnBoard(piece, piece->getPosition().first, piece->getPosition().second);
    }
    game->setCurrentPlayer(game->getPlayerByID(gameState.currentPlayerID));
}

int NetworkClient::receiveIdentifier() {
    int identifier;
    ssize_t received = recv(client_fd, &identifier, sizeof(identifier), 0);
    if (received != sizeof(identifier)) {
        perror("Failed to receive identifier");
        throw std::runtime_error("Error receiving identifier from server.");
    }
    std::cout << "Received identifier: " << identifier << std::endl;
    try {
        sendAcknowledgment();
    } catch (const std::exception& e) {
        std::cerr << "Failed to send acknowledgment receiveIdentifier: " << e.what() << std::endl;
        throw;
    }
    return identifier;
}

int NetworkClient::getPlayerID() {
    return playerID;
}

void NetworkClient::setPlayerID(int playerId) {
    playerID = playerId;
}

void NetworkClient::sendResponseToServer (RequestType type, Json::Value response) {
    Json::Value root;
    root["type"] = static_cast<int>(type);
    root["response"] = response;

    Json::StreamWriterBuilder writer;
    std::string serializedResponse = Json::writeString(writer, root);

    ssize_t sent = send(client_fd, serializedResponse.c_str(), serializedResponse.size(), 0);
    if (sent != static_cast<ssize_t>(serializedResponse.size())) {
        perror("Failed to send response to server");
        throw std::runtime_error("Error sending response to server.");
    }

    //std::cout << "Response sent to server." << std::endl;
    //std::cout << "Waiting fo ACK" << std::endl;
    if (!waitForAcknowledgment()) {
        throw std::runtime_error("Failed to receive acknowledgment sendResponseToServer.");
    }
    //std::cout << "ACK received" << std::endl;
}

ServerRequest NetworkClient::receiveRequest() {
    char buffer[4096];
    ssize_t bytesReceived = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        throw std::runtime_error("Failed to receive request from server.");
    }

    buffer[bytesReceived] = '\0';
    std::string jsonString(buffer);
    //std::cout<<"jsonString receiveRequest:"<<jsonString<<std::endl;
    ServerRequest request;
    try {
        sendAcknowledgment();
    } catch (const std::exception& e) {
        std::cerr << "Failed to send acknowledgment receiveRequest: " << e.what() << std::endl;
        throw;
    }
    return request.deserialize(jsonString);
}

bool NetworkClient::waitForAcknowledgment() {
    char buffer[4096];
    ssize_t bytesReceived = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    /*if (bytesReceived <= 0) {
        throw std::runtime_error("Failed to receive acknowledgment from server.");
    }*/

    buffer[bytesReceived] = '\0';
    std::string response(buffer);

    return response == ACK_MESSAGE; // Retourne true si l'acquittement est reçu
}

void NetworkClient::sendAcknowledgment() {
    ssize_t sent = send(client_fd, ACK_MESSAGE.c_str(), ACK_MESSAGE.size(), 0);
    if (sent != static_cast<ssize_t>(ACK_MESSAGE.size())) {
        throw std::runtime_error("Failed to send acknowledgment to server.");
    }
    //std::cout<<"ACK sent :"<<ACK_MESSAGE<<std::endl;
}

std::string NetworkClient::receiveLargeJson() {
    // Recevez la taille totale
    uint32_t totalSize;
    ssize_t received = recv(client_fd, &totalSize, sizeof(totalSize), 0);
    if (received != sizeof(totalSize)) {
        throw std::runtime_error("Failed to receive total size.");
    }

    totalSize = ntohl(totalSize); // Convertir en ordre d'octets hôte

    // Préparez un buffer pour recevoir les données
    std::string jsonString;
    jsonString.reserve(totalSize);

    char buffer[4096];
    size_t bytesReceived = 0;

    while (bytesReceived < totalSize) {
        size_t remaining = totalSize - bytesReceived;
        size_t currentChunkSize = std::min(sizeof(buffer), remaining);

        received = recv(client_fd, buffer, currentChunkSize, 0);
        if (received <= 0) {
            throw std::runtime_error("Failed to receive JSON chunk.");
        }

        jsonString.append(buffer, received);
        bytesReceived += received;
        //std::cout<<"jsonString :"<<jsonString<<std::endl;
        // Envoyez un acquittement pour chaque chunk
        /*try {
            sendAcknowledgment();
        } catch (const std::exception& e) {
            std::cerr << "Failed to send acknowledgment chunck receiveLargeJson: " << e.what() << std::endl;
            throw;
        }*/
    }

    //std::cout << "JSON file received successfully!" << std::endl;
    try {
        sendAcknowledgment();
    } catch (const std::exception& e) {
        std::cerr << "Failed to send acknowledgment chunck receiveLargeJson: " << e.what() << std::endl;
        throw;
    }
    return jsonString;
}

