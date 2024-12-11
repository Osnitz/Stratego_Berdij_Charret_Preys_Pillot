#ifndef SCENARIOMANAGER_H
#define SCENARIOMANAGER_H

#include <memory>
#include "engine.h"
#include "client.h"
#include "../shared/ai.h"
#include <utility>

template<class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

enum class GameMode {
    PVP,   // Joueur vs Joueur
    PVE,   // Joueur vs IA
    AIvsAI // IA vs IA
};

class ScenarioManager {
private:
    engine::Engine* engine;
    std::unique_ptr<client::PlayerController> player1Controller;
    std::unique_ptr<client::PlayerController> player2Controller;
    GameMode mode;

public:
    ScenarioManager(engine::Engine* eng) : engine(eng), mode(GameMode::PVP) {}

    // Configure le mode de jeu
    void setMode(GameMode m) {
        mode = m;
    }

    // Initialise les PlayerControllers en fonction du mode de jeu
    void initializeControllers() {
        // PlayerController prend un pointeur vers Engine et un playerID
        player1Controller = make_unique<client::PlayerController>(engine, 0);
        player2Controller = make_unique<client::PlayerController>(engine, 1);

        switch (mode) {
        case GameMode::PVP:
            // PvP : Les deux joueurs sont humains
            player1Controller->switchToHuman();
            player2Controller->switchToHuman();
            break;

        case GameMode::PVE: {
            // PvE : Joueur 1 humain, Joueur 2 IA
            player1Controller->switchToHuman();
            ai::AIInterface* ai = new ai::RandomAI(); // Un exemple simple d'IA
            player2Controller->switchToAI(ai);
            break;
        }

        case GameMode::AIvsAI: {
            // AI vs AI : Les deux joueurs sont des IA
            ai::AIInterface* ai1 = new ai::RandomAI();
            ai::AIInterface* ai2 = new ai::RandomAI(); // On peut utiliser la même IA ou une IA différente
            player1Controller->switchToAI(ai1);
            player2Controller->switchToAI(ai2);
            break;
        }
        }
    }
};

#endif
