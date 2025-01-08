#include "server.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "server.h"
using namespace server;
int main() {
    auto server= new Server(8080, false);

    // Démarrer le serveur sur le port 53000
    server->start();

    // Accepter des connexions et envoyer des messages
    std::thread serverThread([&]() {
        while (true) {
            server->acceptClients();

            // Envoyer un message de bienvenue à tous les clients
            std::string welcomeMessage = R"({"status":"connected","message":"Welcome to the server!"})";
            std::string specialMessage = R"({"mtf!"})";
            server->broadcastData(welcomeMessage);
            server->sendData(server->clients[0], specialMessage);


        }
    });

    // Arrêter le serveur après 60 secondes (pour tester)
    std::this_thread::sleep_for(std::chrono::seconds(60));
    server->stop();

    // Attendre que le thread termine
    serverThread.join();

    return 0;
}
