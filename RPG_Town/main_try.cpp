//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include "Player.h"
//#include "Enums.h"
//using namespace std;
//
//void sysMovement(actor::Player&, sf::Clock&, double player_speed);
//
//int main(int argc, char** argv) {
//    
//    // Define parameters for player functions
//    double player_speed = speed::Fast;
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
//    actor::Player actorPlayer(pTexture);
//    
//    // game loop
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//        }
//        
//        window.clear();
//        sysMovement(actorPlayer, gameClock, player_speed);
//        window.draw(actorPlayer);
//        window.display();
//    }
//    return 0;
//}
//
//// Player movement system
//void sysMovement(actor::Player& player, float elapsedTime, float player_speed, bool collision)
//{
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
//        player_speed = speed::Fastest;
//    }
//    else
//        player_speed = speed::Normal;
//    
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
//        player.move(actor::Player::North, elapsedTime, player_speed, collision);
//    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//        player.move(actor::Player::South, elapsedTime, player_speed, collision);
//    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//        player.move(actor::Player::East, elapsedTime, player_speed, collision);
//    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//        player.move(actor::Player::West, elapsedTime, player_speed, collision);
//    }
//    else
//    {
//        player.idle();
//    }
//    
//}