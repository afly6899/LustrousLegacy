#include <SFML/Graphics.hpp>
#include <iostream>
#include "animated_player.h"
#include "Enums.h"
using namespace std;

void updateAll(actor::AnimatedPlayer& player, sf::Clock& clock, bool& animated);

int main() {
    int count = 1;
    bool animated = false;
    
    // window
    sf::RenderWindow window(sf::VideoMode(800, 600), "RPGTown 0.1");
    window.setVerticalSyncEnabled(true);
    
    // clock
    sf::Clock gameClock;
    
    // player texture
    sf::Texture pTexture;
    if (!pTexture.loadFromFile("Eevee.png")) {
        cerr << "Texture Error" << endl;
    }
    
    actor::AnimatedPlayer player(pTexture);
    
    // game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        updateAll(player, gameClock, animated);
        window.draw(player);
        window.display();
    }
    return 0;
}

void updateAll(actor::AnimatedPlayer& player, sf::Clock& clock, bool& animated) {
    if (clock.getElapsedTime().asMilliseconds() >= 200) {
        player.update(animated);
        clock.restart();
    }
}