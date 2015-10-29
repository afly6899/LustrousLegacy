#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "player.h"
#include "Enums.h"
#include "tmx\MapLoader.h"
using namespace std;

void sysMovement(actor::Player&, float elapsedTime, double player_speed, bool collision);
void sysPause(bool& pause, sf::Music& music); 

int main(int argc, char** argv) {

	// Define parameters for player functions
	double player_speed = speed::Fast;
	bool collision = false;
	bool pause = false;
	float elapsedTime = 0;

	// window
	sf::RenderWindow window(sf::VideoMode(800, 600), "RPGTown 0.2");
	window.setVerticalSyncEnabled(true);

	// view (2d camera)
	sf::View playerView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

	// TMX map loader
	tmx::MapLoader ml("\maps");
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
		elapsedTime = gameClock.restart().asMilliseconds();

		cout << pause << endl;
		if (!pause)
		{
			// player movement system and control parameters
			sysMovement(actorPlayer, elapsedTime, player_speed, collision);

			// currently there is an error and only the latest collision object is checked...
			for (auto layer = ml.GetLayers().begin(); layer != ml.GetLayers().end(); ++layer)
			{
				if (layer->name == "Collision")
				{

					for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
					{
						switch (actorPlayer.getDirection()) {
						case Direction::North:
							collision = object->Contains(sf::Vector2f(actorPlayer.getPosition().x + 16, actorPlayer.getPosition().y)) || object->Contains(sf::Vector2f(actorPlayer.getPosition().x - 16, actorPlayer.getPosition().y));
							break;
						case Direction::East:
							collision = object->Contains(sf::Vector2f(actorPlayer.getPosition().x + 32, actorPlayer.getPosition().y + 32));
							break;
						case Direction::South:
							collision = object->Contains(sf::Vector2f(actorPlayer.getPosition().x + 16, actorPlayer.getPosition().y + 32)) || object->Contains(sf::Vector2f(actorPlayer.getPosition().x - 16, actorPlayer.getPosition().y + 32));
							break;
						case Direction::West:
							collision = object->Contains(sf::Vector2f(actorPlayer.getPosition().x - 32, actorPlayer.getPosition().y + 32));
							break;
						}

						if (collision == true)
						{
							actorPlayer.setPosition(actorPlayer.getPastPosition().x, actorPlayer.getPastPosition().y);
						}
					}
				}
			}

			// adjust view to keep it on player
			playerView.setCenter(actorPlayer.getPosition());
		}
		

		window.clear();

		if (collision != true)
		{
			window.setView(playerView);
		}

		// draw map, then draw player

		window.draw(ml);
		window.draw(actorPlayer);
		ml.Draw(window, 4, 0);

		if (pause == true)
		{
			pauseSprite.setPosition(actorPlayer.getPosition());
			window.draw(pauseSprite);
		}
		window.display();
	}
	
	return 0;
}

void sysMovement(actor::Player& player, float elapsedTime, double player_speed, bool collision)
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

void sysPause(bool& pause, sf::Music& music)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		pause = !pause;
		if (pause == true)
		{
			music.pause();
		}
		else if (pause == false)
		{
			music.play();
		}
	}
}
