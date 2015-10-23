#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "player.h"
#include "Enums.h"
#include "tmx\MapLoader.h"
using namespace std;

void sysMovement(actor::Player&, sf::Clock&, double player_speed);

int main(int argc, char** argv) {

	// Define parameters for player functions
	double player_speed = speed::Fast;

	// window
	sf::RenderWindow window(sf::VideoMode(800, 600), "RPGTown 0.2");
	window.setVerticalSyncEnabled(true);

	// view (2d camera)
	sf::View playerView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

	// TMX map loader
	tmx::MapLoader ml("\maps");
	ml.Load("test.tmx");

	// collision variable 
	bool collision = false;

	// clock
	sf::Clock gameClock;

	// player texture
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("playerSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	// give player their texture
	actor::Player actorPlayer(pTexture);

	// set player position on screen
	actorPlayer.setPosition(32, 32);

	// Music!
	sf::Music music;
	if (!music.openFromFile("test.ogg"))
	return -1; // error
	music.play();
	

	// game loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();

		/** map quad tree (still working on this)
		ml.UpdateQuadTree(actorPlayer.getGlobalBounds());
		std::vector<tmx::MapObject*> objects = ml.QueryQuadTree(actorPlayer.getGlobalBounds());
		**/

		// Draw map
		window.draw(ml);

		/* WORKING ON COLLISION STILL
		for (auto layer = ml.GetLayers().begin(); layer != ml.GetLayers().end(); ++layer)
		{
			if (layer->name == "Collision")
			{
				for (auto object = layer->objects.begin(); object != layer->objects.end(); ++object)
				{
					collision = object->Contains(actorPlayer.getPosition());
				}
			}
		}
		*/

		// player movement system and control parameters
		sysMovement(actorPlayer, gameClock, player_speed);

		// adjust view to keep it on player
		playerView.setCenter(actorPlayer.getPosition());
		window.setView(playerView);

		// draw chracter
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
