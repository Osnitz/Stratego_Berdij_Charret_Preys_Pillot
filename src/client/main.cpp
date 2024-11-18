#include <iostream>

// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>

void testSFML() {
    sf::Texture texture;
}
// end of test SFML

#include <state.h>

using namespace std;
using namespace state;

int main() {
    auto game = Game::getInstance();
    game->startGame();
}

/*int main(int argc,char* argv[])
{
    cout << "It works !" << endl;
    return 0;
}*/
