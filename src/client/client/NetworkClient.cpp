#include "client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

using namespace client;

NetworkClient::NetworkClient(std::string server_ip, int port) : client_fd(-1), server_ip(server_ip),server_port(port) {}

NetworkClient::~NetworkClient() {
    disconnect();
}

void NetworkClient::connectToServer() {
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip.c_str(), &server_address.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        return;
    }

    if (connect(client_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return;
    }

    std::cout << "Connected to server." << std::endl;
}

void NetworkClient::disconnect() {
    if (client_fd != -1) {
        close(client_fd);
        client_fd = -1;
    }
}

void NetworkClient::sendData(const std::string& message) {
    send(client_fd, message.c_str(), message.size(), 0);
}

std::string NetworkClient::receiveData() {
    char buffer[1024] = {0};
    ssize_t bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received < 0) {
        perror("Receive failed");
        return "";
    }
    return std::string(buffer, bytes_received);
}
