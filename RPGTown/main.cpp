#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "player.h"
#include "Enums.h"
#include "textbox.h"
#include "tmx\MapLoader.h"
using namespace std;

bool sysMovement(actor::Player& player, int &player_speed, float elapsedTime);
void sysCollision(actor::Player& player, tmx::MapLoader& map, bool& collision, bool& player_trigger, bool& player_event);
void sysPause(bool& pause, sf::Music& music); 
void console_message(string x);

int main(int argc, char** argv) {

	/*
	#--------------------------------------------------------------------------------------------------#
	# Test Parameters: (temporary variables to test functionality of game)
	#
	# test_string - is used to send a message to the textbox that is will display to test word wrapping
	# test_speed - is used to control the text speed of the textbox for testing (F3 controls change)
	#--------------------------------------------------------------------------------------------------#
	*/

	std::string test_string = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
	int test_speed = 0;

	/*
	#--------------------------------------------------------------------------------------------------#
	# Game Window Parameters:
	#
	# window_width	- defines the width of the window
	# window_height - defines the height of the window
	# window_name	- defines the name of the window
	#
	#--------------------------------------------------------------------------------------------------#
	*/

	int window_width = 800;
	int window_height = 600;
	string window_name = "RPGTown 0.3";

	/*
	#--------------------------------------------------------------------------------------------------#
	# System Switches:
	#
	# debug - toggle to display debug information (FPS/Position : F1)
	# textbox - toggle to display textbox (F2)
	# pause - toggle to pause game (esc)
	# player_trigger - is used to notify system to when a player presses ENTER
	# player_event - is used to notify system when player is inside an event tile
	# is_moving - is used to notify the system when a player is moving
	# collision -  is used to stop the player from taking note tiles travelled if colliding with object
	#
	#--------------------------------------------------------------------------------------------------#
	*/

	bool debug = false;
	bool textbox = false;
	bool pause = false;
	bool player_trigger = false;
	bool player_event = false;
	bool is_moving = false;
	bool collision = false;

	/*
	#--------------------------------------------------------------------------------------------------#
	# System Parameters:
	#
	# sysFont - is used to set the font used for text in the game.
	# textDebug - is used to store and display the frames per second of the game 
	#			  as well as other debug information.
	# player_speed - controls the movement speed of the player
	# distance_moved - is used to keep track of the distance the player has moved in order to align 
	#				   player to movement grid
	# elapsedTime - is used to get the amount of time that has passed after every game loop iteration;
	#				elapsed time is used for managing the speed of all animations
	# tilesize - defines the tilesize of the system for readability and operations
	#
	#--------------------------------------------------------------------------------------------------#
	*/

	sf::Font sysFont;
	if (!sysFont.loadFromFile("Vera.ttf"))
	{
		cerr << "Font Error" << endl;
	}

	sf::Text textDebug;
	textDebug.setFont(sysFont);
	textDebug.setCharacterSize(18);

	int player_speed = speed::Normal;
	int distance_moved = 0;
	float elapsedTime = 0;
	int tilesize = 64;

	/*
	#-------- GAME WINDOW --------#
	*/

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_name);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	/*
	#-------- GAME CAMERA AND CLOCK--------#
	*/

	sf::View playerView(sf::FloatRect(0, 0, (float)window_width, (float)window_height));
	sf::Clock gameClock;

	/*
	#-------- TEXTURES --------#
	*/

	// PLAYER TEXTURE
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("playerSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	// PAUSE TEXTURE
	sf::Texture syspTexture;
	if (!syspTexture.loadFromFile("pause.png")) {
		cerr << "Texture Error" << endl;
	}

	// FACE TEXTURES
	sf::Texture pfTexture;
	if (!pfTexture.loadFromFile("face.png")) {
		cerr << "Texture Error" << endl;

	}

	/*
	#-------- MUSIC --------#
	*/
	
	sf::Music music;
	if (!music.openFromFile("test.ogg"))
		return -1; // error
	music.play();

	/*
	#-------- SOUNDS --------#
	*/

	sf::SoundBuffer bleep;
	if (!bleep.loadFromFile("test_sound.wav"))
		return -1; // error

	sf::Sound soundBleep;
	soundBleep.setBuffer(bleep);
	sf::Sound& soundBleep_ref = soundBleep;

	/*
	#--------------------------------------------------------------------------------------------------#
	# World Parameters (Animates backgrounds):
	#
	# aniCounter - used as a decrementer to perform animation
	# aniFrameDuration - used to determine animation speed
	# layer - to keep track of layer to draw
	#--------------------------------------------------------------------------------------------------#
	*/

	float aniCounter = 0;
	float aniFrameDuration = 800;
	int layer = 0;

	/*
	#--------------------------------------------------------------------------------------------------#
	# LOAD MAP
	#
	# Create all maps and load the first map.
	#--------------------------------------------------------------------------------------------------#
	*/

	tmx::MapLoader ml("maps");
	ml.Load("test_new.tmx");

	/*
	#--------------------------------------------------------------------------------------------------#
	# PREPARE CHARACTER
	#
	# Create the player and assign the player texture and set starting position.
	#--------------------------------------------------------------------------------------------------#
	*/

	// SET PLAYER TEXTURE AND POSITION
	actor::Player actorPlayer(pTexture);
	actorPlayer.setPosition(tilesize * 10 + 32, tilesize * 10 + 32);

	/*
	#--------------------------------------------------------------------------------------------------#
	# PREPARE TEXTBOX
	#
	# Create the textbox object and set position based on character.
	#--------------------------------------------------------------------------------------------------#
	*/

	Textbox textBox(pfTexture, sysFont, actorPlayer.getPosition(), 800, 600, 16, soundBleep_ref);

	/*
	#--------------------------------------------------------------------------------------------------#
	# PREPARE SCREENS
	#
	# Set up pause screen.
	#--------------------------------------------------------------------------------------------------#
	*/

	sf::Sprite pauseSprite(syspTexture);
	pauseSprite.setOrigin(400, 300);

	/*
	#--------------------------------------------------------------------------------------------------#
	# BEGIN GAME LOOP:
	#--------------------------------------------------------------------------------------------------#
	*/

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				sysPause(pause, music);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
					debug = !debug;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
					textbox = !textbox;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3)) {
					textBox.setSpeed(test_speed);
					test_speed += 25;
					if (test_speed > 100)
						test_speed = 0;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && player_event)
					player_trigger = !player_trigger;
			}
		}

		// START Get debug information:
		textDebug.setString("FPS: " + to_string(1 / gameClock.getElapsedTime().asSeconds()) + "\nCoordinates: (" + to_string(actorPlayer.getPosition().x) + ", " + to_string(actorPlayer.getPosition().y) + "\nTile Map: (" + to_string(actorPlayer.getPosition().x / tilesize) + ", " + to_string(actorPlayer.getPosition().y / tilesize));
		// END

		// Get the elapsed time from the game clock
		elapsedTime = gameClock.restart().asMilliseconds();

		// if the game is not paused, perform normal game actions
		if (!pause)
		{
			aniCounter += elapsedTime;

			// START - PLAYER GRID CONTROLLER
			if (!is_moving) {
				is_moving = sysMovement(actorPlayer, player_speed, elapsedTime);
				distance_moved = player_speed;
			}
			else if (collision) {
				distance_moved = 0;
				is_moving = false;
				collision = false;
			}
			else if (distance_moved == 32) {
				is_moving = false;
				distance_moved = 0;
			}
			else {
				actorPlayer.move(actorPlayer.getDirection(), player_speed, elapsedTime);
				distance_moved += player_speed;	
			}
			// END 

			// START - COLLISION AND EVENT DETECTION
			sysCollision(actorPlayer, ml, collision, player_trigger, player_event);
			// END 
			
			// after a event has occured, let the system know the event is over
			player_trigger = false;

			// adjust the camera to be viewing player
			playerView.setCenter(actorPlayer.getPosition());
		}
		
		// prepare to update screen
		window.clear();

		//update camera if there isn't a collision
		window.setView(playerView);
	
		// draw animated background (layers 0 and 1 are alternated)
		if (aniCounter >= aniFrameDuration)
		{
			aniCounter -= aniFrameDuration;
			ml.Draw(window, layer);
			layer = layer + 1;
			if (layer > Layer::Background_2)
				layer = Layer::Background_1;
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

		if (textbox)
		{
			if (!pause)
			{
				textBox.setPosition(actorPlayer.getPosition());
				textBox.setFontSize(24);
				textBox.message(test_string, "Warren", elapsedTime);
			}
			window.draw(textBox);
		}

		// if game is paused, draw pause screen
		if (pause)
		{
			pauseSprite.setPosition(actorPlayer.getPosition());
			window.draw(pauseSprite);
		}

		if (debug) {
			textDebug.setPosition(actorPlayer.getPosition().x - 400, actorPlayer.getPosition().y - 300);
			window.draw(textDebug);
		}

		// update screen with changes
		window.display();
	}
	
		return 0;
}

// Player movement system
bool sysMovement(actor::Player& player, int &player_speed, float elapsedTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		player_speed = speed::Fastest;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		player_speed = speed::Fast;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		player_speed = speed::Slow;
	}
	else {
		player_speed = speed::Normal;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.move(actor::Player::North, player_speed, elapsedTime);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.move(actor::Player::South, player_speed, elapsedTime);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.move(actor::Player::East, player_speed, elapsedTime);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.move(actor::Player::West, player_speed, elapsedTime);
	}
	else
	{
		player.idle();
		return false;
	}
	return true;
}

// Collision and Event handling system
void sysCollision(actor::Player& player, tmx::MapLoader& map, bool& collision, bool& player_trigger, bool& player_event)
{
	bool test_collision = false;

	for (auto layer = map.GetLayers().begin(); layer != map.GetLayers().end(); ++layer)
	{
		if (layer->name == "Collision")
		{

			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				switch (player.getDirection()) {
				case Direction::North:
					test_collision = object->Contains(sf::Vector2f(player.getPosition().x + 16, player.getPosition().y - 32)) || object->Contains(sf::Vector2f(player.getPosition().x - 16, player.getPosition().y - 32));
					break;
				case Direction::East:
					test_collision = object->Contains(sf::Vector2f(player.getPosition().x + 31, player.getPosition().y + 16)) || object->Contains(sf::Vector2f(player.getPosition().x + 31, player.getPosition().y + 16));
					break;
				case Direction::South:
					test_collision = object->Contains(sf::Vector2f(player.getPosition().x + 16, player.getPosition().y - 1)) || object->Contains(sf::Vector2f(player.getPosition().x - 16, player.getPosition().y - 1));
					break;
				case Direction::West:
					test_collision = object->Contains(sf::Vector2f(player.getPosition().x - 32, player.getPosition().y + 31)) || object->Contains(sf::Vector2f(player.getPosition().x - 32, player.getPosition().y + 31));
					break;
				}

				if (test_collision)
				{
					console_message("Player has collided with object.");		
					player.setPosition(player.getPastPosition().x, player.getPastPosition().y);
					collision = true;
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
							console_message("START EVENT: " + object->GetName());
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
		if (pause)
		{
			console_message("Game is paused.");
			music.pause();
		}
		else
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
