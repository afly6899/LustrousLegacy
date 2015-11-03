//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include "animated_player.h"
//#include "Enums.h"
//using namespace std;
//
//void updateAll(actor::AnimatedPlayer& player, sf::Clock& clock, int direction);
//bool updateMovement(actor::AnimatedPlayer& player, int& direction);
//
//int main() {
//    int direction = 0;
//    int counter = 0;
//    
//    // window
//    sf::RenderWindow window(sf::VideoMode(800, 600), "RPGTown 0.1");
//    window.setVerticalSyncEnabled(true);
//    
//    // clock
//    sf::Clock gameClock;
//    
//    // player texture
//    sf::Texture pTexture;
//    if (!pTexture.loadFromFile("playerSprite.png")) {
//        cerr << "Texture Error" << endl;
//    }
//    
//    actor::AnimatedPlayer player(pTexture);
//    
//    // game loop
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//        window.clear();
//        updateAll(player, gameClock, direction);
//        window.draw(player);
//        window.display();
//    }
//    return 0;
//}
//
//void updateAll(actor::AnimatedPlayer& player, sf::Clock& clock, int direction) {
//    
//    if (updateMovement(player, direction)) {
//        std::cout << "direction " << direction << std::endl;
//        clock.restart();
//    }
//    
//    else if (player.get_is_moving()) {
//        clock.restart();
//        player.updateMove(direction);
//    }
//    
//    else {
//        
//    }
//    if (clock.getElapsedTime().asMilliseconds() >= 3000) {
//        player.idle();
//        clock.restart();
//    }
//    else if (clock.getElapsedTime().asMilliseconds() >= 2000) {
//        player.updateIdle();
//    }
//}
//
//bool updateMovement(actor::AnimatedPlayer& player, int& direction) {
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
//        direction = actor::AnimatedPlayer::North;
//    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
//        direction = actor::AnimatedPlayer::South;
//    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//        direction = actor::AnimatedPlayer::East;
//    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//        direction = actor::AnimatedPlayer::West;
//    }
//    else {
//        std::cout << "At Else statement, direction is " << direction << std::endl;
//        return false;
//    }
//    player.updateMove(direction);
//    return true;
//}
