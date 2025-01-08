#include <iostream>
//
// Created by matthieu on 08/01/25.
//
#include "client.h"
#include <thread>
using namespace client;
int main() {
    auto client = new NetworkClient("127.0.0.1", 8080);

    // Se connecter au serveur
    client->connectToServer();

    // Envoyer un message au serveur
    std::string message = R"({"action":"greet","playerId":1})";
    client->sendData(message);

    // Recevoir un message du serveur
    std::thread receiveThread([&]() {
        while (true) {
            std::string response = client->receiveData();
            if (!response.empty()) {
                std::cout << "Server: " << response << std::endl;
            }
        }
    });

    // Après 20 secondes, fermer la connexion (pour tester)
    std::this_thread::sleep_for(std::chrono::seconds(20));
    client->disconnect();

    // Attendre que le thread termine
    receiveThread.join();

    return 0;
}
