//
// Created by matthieu on 11/12/24.
//

#include "engine.h"
#include "client.h"
class PlayerController {
private:
    engine::Engine* engine;
    ai::AIInterface* ai;
    int playerId;

public:
    PlayerController(engine::Engine* eng, int id)
        : engine(eng), ai(nullptr), playerId(id) {}

    void switchToAI(ai::AIInterface* newAI) {
        ai = newAI; // pointeur valide vers une IA
    }

    void switchToHuman() {
        ai = nullptr; // plus d'IA
    }

    bool isAI() const {
        return (ai != nullptr);
    }

    void executeCmd(std::pair<int,int> from, std::pair<int,int> to, state::Player* currentPlayer) {
        // Transmet la commande à l’Engine
        bool success = engine->handleCmd(from, to, currentPlayer);
        if (!success) {
            // Gérer l'échec, par exemple afficher un message d'erreur
        }
    }

    int getPlayerId() const {
        return playerId;
    }
};
