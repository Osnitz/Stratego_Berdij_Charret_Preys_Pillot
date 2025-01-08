#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <server.h>

using namespace server;

Server::Server(int port, bool running) : server_fd(-1), port(port), running(running) {}

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
    std::cout << "New client connected." << std::endl;
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
