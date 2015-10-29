#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "player.h"
#include "Enums.h"
#include "tmx\MapLoader.h"
using namespace std;

void sysMovement(actor::Player&, float elapsedTime, float player_speed, bool collision);
void sysCollision(actor::Player& player, tmx::MapLoader& map, bool& collision);
void sysPause(bool& pause, sf::Music& music); 
void console_message(string x);

int main(int argc, char** argv) {

	// Window parameters

	int window_width = 800;
	int window_height = 600;
	string window_name = "RPGTown 0.3";

	// Define parameters for player functions
	float player_speed = speed::Fast;
	bool collision = false;
	bool pause = false;
	float elapsedTime = 0;

	// window
	sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_name);
	window.setVerticalSyncEnabled(true);

	// view (2d camera)
	sf::View playerView(sf::FloatRect(0, 0, (float)window_width, (float)window_height));

	// TMX map loader
	tmx::MapLoader ml("maps");
	ml.Load("test_new.tmx");

	// clock
	sf::Clock gameClock;

	// pause texture
	sf::Texture syspTexture;
	if (!syspTexture.loadFromFile("pause.png")) {
		cerr << "Texture Error" << endl;
	}
	// setting up pause 'screen'
	sf::Sprite pauseSprite(syspTexture);
	pauseSprite.setOrigin(400, 300);

	// player texture
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("playerSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	// give player their texture
	actor::Player actorPlayer(pTexture);

	// set player position on screen
	actorPlayer.setPosition(64*10 + 32, 64*10 + 32);

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
			else if (event.type == sf::Event::KeyPressed) {
				sysPause(pause, music);
			}
		}

		// store how much time has elapsed
		elapsedTime = (float)gameClock.restart().asMilliseconds();

		if (!pause)
		{
			// player movement system and control parameters
			sysMovement(actorPlayer, elapsedTime, player_speed, collision);

			// currently there is an error and only the latest collision object is checked...
			sysCollision(actorPlayer, ml, collision);

			// adjust view to keep it on player
			playerView.setCenter(actorPlayer.getPosition());
		}
		
		// prepare to update screen
		window.clear();

		// update camera if there isn't a collision
		if (collision != true)
		{
			window.setView(playerView);
		}

		// draw map
		window.draw(ml);
		// draw player
		window.draw(actorPlayer);
		// draw top layer of map
		ml.Draw(window, 4, 0);

		// if game is paused, draw pause screen
		if (pause == true)
		{
			pauseSprite.setPosition(actorPlayer.getPosition());
			window.draw(pauseSprite);
		}
		
		// update screen with changes
		window.display();
	}
	
	return 0;
}

// Player movement system
void sysMovement(actor::Player& player, float elapsedTime, float player_speed, bool collision)
{
	// currently, game clock is passed to player object (probably don't want to do this)

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.move(actor::Player::North, elapsedTime, player_speed, collision);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.move(actor::Player::South, elapsedTime, player_speed, collision);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.move(actor::Player::East, elapsedTime, player_speed, collision);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.move(actor::Player::West, elapsedTime, player_speed, collision);
	}
	else
	{
		player.idle();
	}

}

// Collision detection system
void sysCollision(actor::Player& player, tmx::MapLoader& map, bool& collision)
{
	for (auto layer = map.GetLayers().begin(); layer != map.GetLayers().end(); ++layer)
	{
		if (layer->name == "Collision")
		{

			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				switch (player.getDirection()) {
				case Direction::North:
					collision = object->Contains(sf::Vector2f(player.getPosition().x + 16, player.getPosition().y)) || object->Contains(sf::Vector2f(player.getPosition().x - 16, player.getPosition().y));
					break;
				case Direction::East:
					collision = object->Contains(sf::Vector2f(player.getPosition().x + 32, player.getPosition().y + 32));
					break;
				case Direction::South:
					collision = object->Contains(sf::Vector2f(player.getPosition().x + 16, player.getPosition().y + 32)) || object->Contains(sf::Vector2f(player.getPosition().x - 16, player.getPosition().y + 32));
					break;
				case Direction::West:
					collision = object->Contains(sf::Vector2f(player.getPosition().x - 32, player.getPosition().y + 32));
					break;
				}

				if (collision == true)
				{
					console_message("Player has collided with object.");
					player.setPosition((int)player.getPastPosition().x, (int)player.getPastPosition().y);
				}
			}
		}
	}
}

// Pause system
void sysPause(bool& pause, sf::Music& music)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		pause = !pause;
		if (pause == true)
		{
			console_message("Game is paused.");
			music.pause();
		}
		else if (pause == false)
		{
			console_message("Game has resumed.");
			music.play();
		}
	}
}

// This function is strictly for debugging purposes (use this to create console messages)
void console_message(string x) {
	cout << "\nSYSTEM MESSAGE: " + x << endl;
}