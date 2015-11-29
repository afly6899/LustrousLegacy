#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"
#include "Enums.h"
#include "textbox.h"
#include "title.h"
#include "debug.h"
#include "fader.h"
#include "NPC.h"
#include "SceneReader.h"
#include "pause.h"
#include "tmx\MapLoader.h"
using namespace std;

/*
#--------------------------------------------------------------------------------------------------#
Game Project: RPGTown (Lustrous Legacy) (v0.3)
#--------------------------------------------------------------------------------------------------#

UC Irvine - Fall 2015 Quarter (current)

#--------------------------------------------------------------------------------------------------#
*/

// main game loop control functions
void sysCollision(Player& player, tmx::MapLoader& map, bool& collision, bool& player_trigger, bool& player_event);
void sysPause(bool& pause, bool& intro, bool& title, sf::Music& music, Fader& sysFader);
sf::Vector2f window_topleft(sf::Vector2f center_pos);
sf::Vector2f tile(int tile_num);

int main(int argc, char** argv) {

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
	string window_name = "Lustrous Legacy (Prototype)";

	/*
	#-------- GAME WINDOW --------#
	*/

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_name);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);

	/*
	#-------- GAME CAMERA AND CLOCK--------#
	*/

	sf::View playerView(sf::FloatRect(0, 0, (float)window_width, (float)window_height));
	sf::Clock gameClock;

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
	#
	#--------------------------------------------------------------------------------------------------#
	*/

	// load the system font
	sf::Font sysFont;
	if (!sysFont.loadFromFile("Vera.ttf")) {
		cerr << "Font Error" << endl;
	}

	// debug text instantiation
	sf::Text textDebug;
	textDebug.setFont(sysFont);
	textDebug.setCharacterSize(Font_Size::Large);

	// basic default player parameters and time since last loop iteration
	int player_speed = Speed::Normal;
	float elapsedTime = 0;

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
	# move_flag - to determine manual or automatic movement
	#--------------------------------------------------------------------------------------------------#
	*/

	bool debug = false;
	bool textbox = false;
	bool player_trigger = false;
	bool player_event = false;
	bool is_moving = false;
	bool collision = false;
	bool move_flag = false;
	bool title = true;
	bool pause = false;
	bool intro = false;

	/*
	#-------- TEXTURES --------#
	*/

	// PLAYER TEXTURE
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("playerSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	// FACE TEXTURES
	sf::Texture pfTexture;
	if (!pfTexture.loadFromFile("face_warren.png")) {
		cerr << "Texture Error" << endl;
	}

	// TITLE BACKGROUND TEXTURE
	sf::Texture bgtitleTexture;
	if (!bgtitleTexture.loadFromFile("title.png")) {
		cerr << "Texture Error" << endl;
	}

	// TITLE TEXTURE
	sf::Texture titleTexture;
	if (!titleTexture.loadFromFile("LustrousLegacyLogo.png")) {
		cerr << "Texture Error" << endl;
	}

	// CURSOR TEXTURE
	sf::Texture cursorTexture;
	if (!cursorTexture.loadFromFile("cursor.png")) {
		cerr << "Texture Error" << endl;
	}

	// BOOK TEXTURE
	sf::Texture bookTexture;
	if (!bookTexture.loadFromFile("book.png")) {
		cerr << "Texture Error" << endl;
	}

	/*
	#-------- MUSIC --------#
	*/
	
	sf::Music music;
	if (!music.openFromFile("test.ogg"))
		return -1; // error

	//music.setVolume(0);

	/*
	#-------- SOUNDS --------#
	*/

	sf::SoundBuffer bleep;
	if (!bleep.loadFromFile("text_blip.wav"))
		return -1; // error

	sf::SoundBuffer selection_bleep;
	if (!selection_bleep.loadFromFile("select_blip.wav"))
		return -1; // error

	sf::Sound soundBleep;
	sf::Sound soundSelect;
	soundBleep.setBuffer(bleep);
	soundSelect.setBuffer(selection_bleep);

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
	ml.Load("start.tmx");

	/*
	#--------------------------------------------------------------------------------------------------#
	# PREPARE CHARACTER
	#
	# Create the player and assign the player texture and set starting position.
	#--------------------------------------------------------------------------------------------------#
	*/

	// SET PLAYER TEXTURE AND POSITION
	Player actorPlayer(pTexture);
	actorPlayer.setPosition(tile(10));

	/*
	#--------------------------------------------------------------------------------------------------#
	# PREPARE TEXTBOX
	#
	# Create the textbox object and set position based on character.
	#--------------------------------------------------------------------------------------------------#
	*/

	Textbox textBox(sysFont, soundBleep, pfTexture, window_width, window_height);

	/*
	#--------------------------------------------------------------------------------------------------#
	# PREPARE SCREENS
	#
	# Set up pause screen.
	#--------------------------------------------------------------------------------------------------#
	*/

	//test//
	Title screenTitle(titleTexture, bgtitleTexture, cursorTexture, sysFont, soundBleep, window_width, window_height);
	Pause screenPause(sysFont, window_width, window_width);
	SceneReader* reader = new SceneReader("Scenes.txt", "Intro");
	Fader sysFader;
	Textbox* introTextbox = nullptr;
	NPC book(bookTexture);
	
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
			else if (event.type == sf::Event::Resized)
			{
				window_width = event.size.width;
				window_height = event.size.height;
				window.setSize(sf::Vector2u(800, 600));
			}
			else if (event.type == sf::Event::KeyPressed) {
				sysPause(pause, intro, title, music, sysFader);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
					debug = !debug;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
					textbox = !textbox;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3) && pause) {
					title = true;
					delete reader;
					reader = new SceneReader("Scenes.txt", "Intro");
					screenTitle.setPosition(actorPlayer.getPosition());
					music.stop();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && player_event)
					player_trigger = !player_trigger;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && title) {
					screenTitle.change_selection(4, Cursor_Direction::Down);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && title) {
					screenTitle.change_selection(4, Cursor_Direction::Up);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && screenTitle.getSelection() == Selection::Play_Game) {
					sysFader.resetFader();
					actorPlayer.setPosition(tile(10));
					actorPlayer.setDirection(Direction::South);
					title = false;
					intro = true; 
					pause = false;
					introTextbox = new Textbox(sysFont, soundBleep, pfTexture, window_width, window_height, true);
					introTextbox->setPosition(actorPlayer.getPosition());
					if (!pause) {
						music.play();
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && screenTitle.getSelection() == 4) {
					window.close();
				}
			}
		}

		// START Get debug information:
		textDebug.setString("FPS: " + to_string(1 / gameClock.getElapsedTime().asSeconds()).substr(0, 5) + 
			"\nCoordinates: (" + to_string(actorPlayer.getPosition().x).substr(0, 5) + ", " + 
			to_string(actorPlayer.getPosition().y).substr(0, 5) + ")\nTile Map: (" + 
			to_string(actorPlayer.getPosition().x / Tilesize).substr(0, 5) + ", " + 
			to_string(actorPlayer.getPosition().y / Tilesize).substr(0, 5) + ")");
		// END debug information

		//PRIME THE CAMERA
		if (!title)
		{
			playerView.setCenter(actorPlayer.getPosition());
			window.setView(playerView);
		}
		
		// Get the elapsed time from the game clock
		elapsedTime = gameClock.restart().asMilliseconds();
		if (!pause) {
			aniCounter += elapsedTime;
		}

		// if the game is not paused, perform normal game actions
		if (!pause && !title && window.hasFocus() && sysFader.isComplete())
		{
			// START - PLAYER MOVEMENT (manual or automatic)
			actorPlayer.move(player_speed, elapsedTime, collision, move_flag);
			// END 

			// START - COLLISION AND EVENT DETECTION
			sysCollision(actorPlayer, ml, collision, player_trigger, player_event);
			// END 

			// adjust the camera to be viewing player
			playerView.setCenter(actorPlayer.getPosition());

			// update textBox position after movement
			textBox.setPosition(playerView.getCenter());
		}

		// prepare to update screen
		window.clear();

		//update camera
		window.setView(playerView);

		// draw animated background (layers 0 and 1 are alternated)
		if (aniCounter >= aniFrameDuration)
		{
			aniCounter -= aniFrameDuration;
			ml.Draw(window, layer);
			layer += 1;
			if (layer > Layer::Background_2)
				layer = Layer::Background_1;
		}
		else
		{
			ml.Draw(window, layer, Background_1);
		}

		// draw walkable and collidable tiles
		ml.Draw(window, Layer::Field);
		ml.Draw(window, Layer::Collision_Objects);

		// draw player
		window.draw(actorPlayer);
		// draw top layer of map
		ml.Draw(window, Layer::Overlay);

		if (textbox && !title && sysFader.isComplete())
		{
			if (!pause && window.hasFocus())
			{
				textBox.setPosition(playerView.getCenter());
				textBox.setFontSize(Font_Size::Large);
				if (!textBox.if_endMessage())
					textBox.message(reader->currentMessage().second, reader->currentMessage().first, elapsedTime);
				else
					{
						textBox.reset();
						if (!reader->isEmpty())
							reader->nextMessage();
						if (reader->isEmpty()) {
							delete reader;
							reader = new SceneReader("Scenes.txt", "Scene1");
						}
							
					}
			}
			window.draw(textBox);
		}
		if (pause)
		{
			screenPause.setPosition(playerView.getCenter());
			window.draw(screenPause);
		}
		if (title) {
			screenTitle.animate(elapsedTime);
			window.draw(screenTitle);

		}
		else if (!title && !intro) {
			sysFader.setPosition(playerView.getCenter());
			sysFader.performFade(0, 1);
			window.draw(sysFader);
		}
		else if (intro)
		{
			introTextbox->setPosition(playerView.getCenter());
			sysFader.setPosition(playerView.getCenter());
			window.draw(sysFader.blackScreen());
			book.setPosition(playerView.getCenter());
			book.hover(elapsedTime);
			window.draw(book);
			if (!introTextbox->if_endMessage())
				introTextbox->message(reader->currentMessage().second, reader->currentMessage().first, elapsedTime);
			else
			{
				introTextbox->reset();
				if (!reader->isEmpty())
					reader->nextMessage();
				if (reader->isEmpty()) {
					intro = false;
					pause = false;
					delete introTextbox;
					introTextbox = nullptr;
					delete reader;
					reader = new SceneReader("Scenes.txt", "Scene1");
				}
			}
		}
		if (intro)
			window.draw(*introTextbox);
		// END HARD-CODED ALPHA PREVIEW
		
		if (debug) {
			textDebug.setPosition(window_topleft(playerView.getCenter()));
			window.draw(textDebug);
		}

		// update screen with changes
		window.display();
	}
		return 0;
}

// Collision and Event handling system
void sysCollision(Player& player, tmx::MapLoader& map, bool& collision, bool& player_trigger, bool& player_event)
{
	bool test_collision = false;
	
	for (auto layer = map.GetLayers().begin(); layer != map.GetLayers().end(); ++layer)
	{
		if (layer->name == "Collision")
		{

			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
	
				sf::Vector2f left = sf::Vector2f(player.getPosition().x - 32, player.getPosition().y);
				sf::Vector2f right = sf::Vector2f(player.getPosition().x + 31, player.getPosition().y);
				sf::Vector2f bottom = sf::Vector2f(player.getPosition().x, player.getPosition().y + 31);
				sf::Vector2f top = sf::Vector2f(player.getPosition().x, player.getPosition().y - 32);

				test_collision = object->Contains(left) || object->Contains(right) || object->Contains(bottom) || object->Contains(top);
				if (test_collision)
				{
					console_message("Player has collided with object.");		
					player.setPosition(player.getPastPosition());
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
					if (sf::Vector2f(object->GetPosition().x + 32, object->GetPosition().y + 32) == player.getPosition())
						player_event = true;
				}
			}
		}
	}
}

// Pause system
void sysPause(bool& pause, bool& intro, bool& title, sf::Music& music, Fader& sysFader)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !intro &&!title && sysFader.isComplete()) {
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

// returns the center position of the tile specified
sf::Vector2f tile(int tile_num) {
	int temp = System::Tilesize*tile_num + 32;
	return sf::Vector2f(temp, temp);
}

// returns the top left corner of the window based on the position provided (we assume center of the screen)
sf::Vector2f window_topleft(sf::Vector2f center_pos) {
	return sf::Vector2f(center_pos.x - 400, center_pos.y - 300);
}