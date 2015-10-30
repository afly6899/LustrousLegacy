#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "player.h"
#include "Enums.h"
#include "tmx\MapLoader.h"
using namespace std;

void sysMovement(actor::Player&, float elapsedTime, float player_speed, bool collision);
void sysCollision(actor::Player& player, tmx::MapLoader& map, bool& collision, bool& player_trigger, bool& player_event);
void sysPause(bool& pause, sf::Music& music); 
void console_message(string x);

int main(int argc, char** argv) {

	// Window parameters

	int window_width = 800;
	int window_height = 600;
	string window_name = "RPGTown 0.3";

	//-------------  Debug tools and TESTS -----------------//
	sf::Font Vera;
	bool debug = false;
	bool textbox = false;

	if (!Vera.loadFromFile("Vera.ttf"))
	{
		cerr << "Font Error" << endl;
	}

	sf::Text FPS;
	FPS.setFont(Vera);
	FPS.setCharacterSize(24);

	sf::RectangleShape rectText;
	rectText.setSize(sf::Vector2f(window_width - 25, window_height*.3));
	rectText.setOrigin((window_width - 25)*.5, window_height*.5);
	rectText.setFillColor(sf::Color::Black);
	rectText.setOutlineColor(sf::Color::White);
	rectText.setOutlineThickness(2);

	//------------- EMD DEBUG TOOLS AND TEST --------------//

	// Define parameters for player functions
	float player_speed = speed::Normal;
	float aniCounter = 0;
	float aniFrameDuration = 800;
	float elapsedTime = 0;
	int layer = 0;
	int tilesize = 64;
	bool collision = false;
	bool pause = false;
	bool player_trigger = false;
	bool player_event = false;

	// window
	sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_name);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

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
	actorPlayer.setPosition(tilesize*10 + 32, tilesize*10 + 32);

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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
					debug = !debug;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2)) {
					textbox = !textbox;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && player_event) {
					player_trigger = !player_trigger;
				}
			}
		}

		// get FPS
		FPS.setString(to_string(1/(float)gameClock.getElapsedTime().asSeconds()));
		// store how much time has elapsed
		elapsedTime = (float)gameClock.restart().asMilliseconds();

		if (!pause)
		{
			aniCounter += elapsedTime;

			// player movement system and control parameters
			sysMovement(actorPlayer, elapsedTime, player_speed, collision);

			// player collision and event system
			sysCollision(actorPlayer, ml, collision, player_trigger, player_event);

			player_trigger = false;

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

		// draw animated background (layers 0 and 1 are alternated)
		if (aniCounter >= aniFrameDuration)
		{
			aniCounter -= aniFrameDuration;
			ml.Draw(window, layer);
			layer = layer + 1;
			if (layer > Layer::Background_2)
			{
				layer = Layer::Background_1;
			}
		}
		else
		{
			ml.Draw(window, layer, 0);
		}

		// draw walkable and collidable tiles
		ml.Draw(window, Layer::Field);
		ml.Draw(window, Layer::Collision_Objects);

		// draw player
		window.draw(actorPlayer);
		// draw top layer of map
		ml.Draw(window, Layer::Overlay);

		// if game is paused, draw pause screen
		if (pause)
		{
			pauseSprite.setPosition(actorPlayer.getPosition());
			window.draw(pauseSprite);
		}

		if (textbox)
		{
			rectText.setPosition(actorPlayer.getPosition().x, actorPlayer.getPosition().y + 410);
			window.draw(rectText);
		}

		if (debug) {
			FPS.setPosition(actorPlayer.getPastPosition().x - 100, actorPlayer.getPosition().y - 100);
			window.draw(FPS);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		player_speed = speed::Fastest;
	}
	else
		player_speed = speed::Normal;

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

// Collision and Event handling system
void sysCollision(actor::Player& player, tmx::MapLoader& map, bool& collision, bool& player_trigger, bool& player_event)
{
	for (auto layer = map.GetLayers().begin(); layer != map.GetLayers().end(); ++layer)
	{
		if (layer->name == "Collision")
		{

			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				switch (player.getDirection()) {
				case Direction::North:
					collision = object->Contains(sf::Vector2f(player.getPosition().x + 8, player.getPosition().y)) || object->Contains(sf::Vector2f(player.getPosition().x - 8, player.getPosition().y));
					break;
				case Direction::East:
					collision = object->Contains(sf::Vector2f(player.getPosition().x + 32, player.getPosition().y + 32));
					break;
				case Direction::South:
					collision = object->Contains(sf::Vector2f(player.getPosition().x + 8, player.getPosition().y + 32)) || object->Contains(sf::Vector2f(player.getPosition().x - 8, player.getPosition().y + 32));
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
		if (layer->name == "Events")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				if ((object->GetName() == "Start"))
				{
					if ((object->Contains(player.getPosition())))
						player_event = true;
					else
						player_event = false;

						if ((player_trigger) && (player.getDirection() == Direction::East))
						{
							cout << "This shit works!" << endl;
						}
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