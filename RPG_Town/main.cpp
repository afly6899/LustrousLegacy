#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "Enums.h"
using namespace std;

void sysMovement(actor::Player&, sf::Clock&, double player_speed);

int main(int argc, char** argv) {

	// Define parameters for player functions
	double player_speed = speed::Fast;

	// window
	sf::RenderWindow window(sf::VideoMode(800, 600), "RPGTown 0.1");
	window.setVerticalSyncEnabled(true);

	// clock
	sf::Clock gameClock;

	// player texture
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("playerSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	actor::Player actorPlayer(pTexture);

	// game loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		sysMovement(actorPlayer, gameClock, player_speed);
		window.draw(actorPlayer);
		window.display();
	}
	return 0;
}

void sysMovement(actor::Player& player, sf::Clock& Clock, double player_speed)
{
	// currently, game clock is passed to player object (probably don't want to do this)

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		player.move(actor::Player::North, Clock, player_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		player.move(actor::Player::South, Clock, player_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		player.move(actor::Player::East, Clock, player_speed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		player.move(actor::Player::West, Clock, player_speed);
	}
	else
	{
		player.idle(Clock);
	}

}
