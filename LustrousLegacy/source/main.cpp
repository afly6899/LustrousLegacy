/*********************************************************************
Game: Lustrous Legacy (RPGTown)
UC Irvine - Fall 2015 Quarter (VGDC Project)

Team:
- Hayden McFarland (Project Lead / Programmer)
- Jonathan Mayer (Programmer)
- Audrey Fu Lai (Programmer)
- Darion Downsen (Writer)
- Diego Guzman (Designer)
- Mark Pareja (Artist)
- Kristina Wong (Sound Engineer)

Libraries used:
SFML - http://www.sfml-dev.org/

Matt Marchant 2013 - 2015
SFML Tiled Map Loader - https://github.com/bjorn/tiled/wiki/TMX-Map-Format
http://trederia.blogspot.com/2013/05/tiled-map-loader-for-sfml.html

*********************************************************************/

// Include libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <math.h>
#include <ltbl/lighting/LightSystem.h>
#include "tmx/MapLoader.h"
// Class definitions
#include "SceneReader.h"
#include "Enums.h"
#include "textbox.h"
#include "title.h"
#include "pause.h"
#include "fader.h"
#include "TutorialEvent.h"
#include "debug/debug.h"
#include "Actor.h"
#include "Character.h"
using namespace std;

void sysCollision(Character& player, Actor& test, tmx::MapLoader& map, bool& collision, bool& player_event);
sf::Vector2f tile(int tile_row, int tile_column);
sf::Vector2f Normalize2f(sf::Vector2f pos);

int main() {

	/*********************************************************************
	GAME WINDOW PARAMETERS:
	window_width	- defines the width of the window
	window_height - defines the height of the window
	window_name	- defines the name of the window
	*********************************************************************/

	int window_width = 800;
	int window_height = 600;
	string window_name = "Lustrous Legacy (Prototype)";

	/*********************************************************************
	GAME WINDOW
	*********************************************************************/

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), window_name);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	/*********************************************************************
	GAME CAMERA AND CLOCK
	*********************************************************************/

	sf::View playerView(sf::FloatRect(0, 0, (float)window_width, (float)window_height));
	sf::Clock gameClock;

	/*********************************************************************
	SYSTEM PARAMETERS:
	sysFont - is used to set the font used for text in the game.
	textDebug - is used to store and display the frames per second of the game as well as other debug information.
	player_speed - controls the movement speed of the player
	distance_moved - is used to keep track of the distance the player has moved in order to align player to movement grid
	elapsedTime - is used to get the amount of time that has passed after every game loop iteration; elapsed time is used for managing the speed of all animations
	*********************************************************************/

	sf::Font sysFont;
	if (!sysFont.loadFromFile("resources/font/Vera.ttf")) {
		cerr << "Font Error" << endl;
	}

	sf::Text textDebug;
	textDebug.setFont(sysFont);
	textDebug.setCharacterSize(Font_Size::Large);

	int player_speed = Speed::Normal;
	float elapsedTime = 0;

	/*********************************************************************
	SYSTEM SWITCHES:
	debug - toggle to display debug information (F1)
	textbox - toggle to display textbox (F2)
	pause - toggle to pause game (esc)
	player_trigger - is used to notify system to when a player presses ENTER
	player_event - is used to notify system when player is inside an event tile
	is_moving - is used to notify the system when a player is moving
	collision -  is used to stop the player from taking note of tiles travelled if colliding with object
	move_flag - to determine manual or automatic movement
	*********************************************************************/

	bool debug = false;
	bool textbox = false;
	bool player_event = false;
	bool is_moving = false;
	bool collision = false;
	bool move_flag = false;
	bool title = true;
	bool pause = false;
	bool intro = false;

	/*********************************************************************
	TEXTURES
	*********************************************************************/

	// PLAYER TEXTURE
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("resources/textures/playerSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	// NPC TEXTURE
	sf::Texture npcTexture;
	if (!npcTexture.loadFromFile("resources/textures/tempSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	// WARREN FACE TEXTURE
	sf::Texture pfTexture;
	if (!pfTexture.loadFromFile("resources/textures/face_warren.png")) {
		cerr << "Texture Error" << endl;
	}

	// RESDIN FACE TEXTURE
	sf::Texture resdinfTexture;
	if (!resdinfTexture.loadFromFile("resources/textures/face_resdin.png")) {
		cerr << "Texture Error" << endl;
	}

	// LUKE FACE TEXTURE
	sf::Texture lukefTexture;
	if (!lukefTexture.loadFromFile("resources/textures/face_luke.png")) {
		cerr << "Texture Error" << endl;
	}

	// TITLE BACKGROUND TEXTURE
	sf::Texture bgtitleTexture;
	if (!bgtitleTexture.loadFromFile("resources/textures/title.png")) {
		cerr << "Texture Error" << endl;
	}

	// TITLE TEXTURE
	sf::Texture titleTexture;
	if (!titleTexture.loadFromFile("resources/textures/LustrousLegacyLogo.png")) {
		cerr << "Texture Error" << endl;
	}

	// CURSOR TEXTURE
	sf::Texture cursorTexture;
	if (!cursorTexture.loadFromFile("resources/textures/cursor.png")) {
		cerr << "Texture Error" << endl;
	}

	// BOOK TEXTURE
	sf::Texture bookTexture;
	if (!bookTexture.loadFromFile("resources/textures/book.png")) {
		cerr << "Texture Error" << endl;
	}

	std::map<std::string, sf::Sprite> faceMap;
	faceMap["Warren"] = sf::Sprite(pfTexture);
	faceMap["Warren"].setOrigin(faceMap["Warren"].getLocalBounds().width*.5, faceMap["Warren"].getLocalBounds().height*.5);
	faceMap["Resdin"] = sf::Sprite(resdinfTexture);
	faceMap["Resdin"].setOrigin(faceMap["Resdin"].getLocalBounds().width*.5, faceMap["Resdin"].getLocalBounds().height*.5);
	faceMap["Luke"] = sf::Sprite(lukefTexture);
	faceMap["Luke"].setOrigin(faceMap["Luke"].getLocalBounds().width*.5, faceMap["Luke"].getLocalBounds().height*.5);

	/*********************************************************************
	MUSIC
	*********************************************************************/

	sf::Music music;
	if (!music.openFromFile("resources/audio/test.wav"))
		return -1; // error

	music.setVolume(50);

	/*********************************************************************
	SOUNDS
	*********************************************************************/

	sf::SoundBuffer bleep;
	if (!bleep.loadFromFile("resources/audio/text_blip.wav"))
		return -1; // error

	sf::SoundBuffer bleep2;
	if (!bleep2.loadFromFile("resources/audio/text_blip2.wav"))
		return -1; // error

	sf::Sound soundBleep;
	sf::Sound soundBleep2;
	soundBleep.setBuffer(bleep);
	soundBleep2.setBuffer(bleep2);

	/*********************************************************************
	WORLD PARAMETERS:
	aniCounter - used as a decrementer to perform animation
	aniFrameDuration - used to determine animation speed
	layer - to keep track of layer to draw
	*********************************************************************/

	float aniCounter = 0;
	float aniFrameDuration = 800;
	int layer = 0;

	/*********************************************************************
	LOAD MAP
	Create all maps and load the first map.
	*********************************************************************/

	tmx::MapLoader ml("resources/maps");
	ml.Load("start.tmx");

	/*********************************************************************
	PREPARE CHARACTER
	Create the player and assign the player texture and set starting position.
	*********************************************************************/

	// SET PLAYER TEXTURE AND POSITION

	/*********************************************************************
	PREPARE TEXTBOX
	Create the textbox object and set position based on character.
	*********************************************************************/

	Textbox textBox(faceMap, sysFont, soundBleep, window_width, window_height);

	/*********************************************************************
	PREPARE SCREENS, ANIMATIONS, AND UTILITIES
	*********************************************************************/

	Title screenTitle(titleTexture, bgtitleTexture, cursorTexture, sysFont, soundBleep, window_width, window_height);
	Pause screenPause(sysFont, window_width, window_width);
	SceneReader* reader = new SceneReader("resources/script/scenes.txt", "Scene1");
	Fader sysFader;
	Textbox* _Textbox = new Textbox(faceMap, sysFont, soundBleep, window_width, window_height);

	/*********************************************************************
	HARD CODED PROTOTYPE PREVIEW
	*********************************************************************/
	// Audrey's trying to do game event stuff
	bool event_start = true; // only turns true if press Enter at that start box thing
							  // once true, should have the character move down 2 spaces
							  //    int start_pos, end_pos; //once it reaches 2, stop event
							  //    Direction event_move = Direction::South;
	Event tutorial = { { Direction::South, System::Tilesize * 7} ,{ Direction::East, System::Tilesize * 3 },{ Direction::North,System::Tilesize } };
	std::string scene_name = "Scene1";

	/*********************************************************************
	LIGHTING SYSTEM TEST
	*********************************************************************/

	// OTHER TEST

	Character player(pTexture);
	Actor test_follow(pTexture);
	Actor test_follow1(pTexture);
	Actor test_follow2(pTexture);
	Actor test_follow3(pTexture);
	player.setPosition(tile(10,10));
	test_follow.setPosition(tile(10,15));
	test_follow1.setPosition(tile(9, 16));
	test_follow2.setPosition(tile(11, 14));
	test_follow3.setPosition(tile(11, 13));
	bool stop = false;

	/*********************************************************************
	BEGIN GAME LOOP:
	*********************************************************************/

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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) &&!title) {
					pause = !pause;
					if (pause)
						music.pause();
					else
						music.play();
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
					debug = !debug;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && title) {
					screenTitle.change_selection(4, Cursor_Direction::Down);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && title) {
					screenTitle.change_selection(4, Cursor_Direction::Up);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && screenTitle.getSelection() == Selection::Play_Game) {
					sysFader.resetFader();
					title = false;
					intro = false;
					if (!pause) {
						music.play();
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && screenTitle.getSelection() == Selection::Exit) {
					window.close();
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2) && !pause && !title)
					textbox = !textbox;
		}
	}

		if ((music.getStatus() == sf::Music::Stopped) && !title && !pause) {
			music.play();
		}

		// prime the camera
		if (!title)
		{
			player.move(elapsedTime, player.getPlayerController().get_input());
			window.setView(playerView);
			// test //
			sf::Vector2f player_direction = Normalize2f(player.getPosition() - test_follow.getPosition());
			int direction;
			test_follow.setSpeed(2);

			if (player_direction.y == 1) {
				if (player_direction.x == 0) {
					direction = Direction::South;
				}
				if (player_direction.x == 1)
					direction = Direction::SouthEast;
				if (player_direction.x == -1)
					direction = Direction::SouthWest;
			}
			else if (player_direction.y == -1) {
				if (player_direction.x == 0) {
					direction = Direction::North;
				}
				else if (player_direction.x == 1)
					direction = Direction::NorthEast;
				else if (player_direction.x == -1)
					direction = Direction::NorthWest;
			}
			else if (player_direction.x == 0) {
				if (player_direction.y == 1)
					direction = Direction::South;
				else if (player_direction.y == -1)
					direction = Direction::North;
			}
			else if (player_direction.y == 0) {
				if (player_direction.x == 1)
					direction = Direction::East;
				else if (player_direction.x == -1)
					direction = Direction::West;
			}

			test_follow.move(elapsedTime, direction);


			// test end //
		}

		// get the elapsed time from the game clock
		elapsedTime = gameClock.restart().asMilliseconds();

		if (!pause) {
			aniCounter += elapsedTime;
		}

		// if the game is not paused, perform normal game actions
		if (!pause && !title && window.hasFocus())
		{

			// START - COLLISION AND EVENT DETECTION (remove scene2_complete when redoing intro)
			sysCollision(player, test_follow, ml, collision, player_event);
			// END 

			// adjust the camera to be viewing player
			playerView.setCenter(player.getPosition());

			if (!_Textbox->if_endMessage())
				_Textbox->message(reader->currentMessage().second, reader->currentMessage().first, elapsedTime);
			else
			{
				_Textbox->reset();
				if (!reader->isEmpty())
					reader->nextMessage();
				if (reader->isEmpty()) {
					_Textbox->reset();
					delete reader;
					reader = new SceneReader("resources/script/scenes.txt", scene_name);
				}
			}
		}



		// prepare to update screen
		window.clear();

		// update camera
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
		if (player.getDepth() > test_follow.getDepth()) {
			window.draw(player);
			window.draw(test_follow);
			window.draw(test_follow1);
			window.draw(test_follow2);
			window.draw(test_follow3);
		}
		else {

			window.draw(test_follow);
			window.draw(test_follow1);
			window.draw(test_follow2);
			window.draw(test_follow3);
			window.draw(player);
			
		}

		// draw top layer of map
		ml.Draw(window, Layer::Overlay);

		if (title) {
			screenTitle.animate(elapsedTime);
			window.draw(screenTitle);

		}
		if (textbox)
		{
			_Textbox->setPosition(playerView.getCenter());
			window.draw(*_Textbox);
		}
			
		if (pause)
		{
			screenPause.setPosition(playerView.getCenter());
			window.draw(screenPause);
		}
		
		if (debug) {
			textDebug.setPosition(playerView.getCenter().x - window_width*.5, playerView.getCenter().y - window_height*.5);
			window.draw(textDebug);
		}

		// update screen with changes
		window.display();
	}
		return 0;
}

/*********************************************************************
\brief Performs collision and event handling.
1. Determines if a player has collided with an object and returns the player to their previous position if true.
2. Determines if a player has entered an event tile and has initiated event.

\param Player, Map, Collision Switch, Player Use Switch, Player Event Switch
*********************************************************************/
void sysCollision(Character& character, Actor& test, tmx::MapLoader& map, bool& collision, bool& player_event)
{
	bool test_collision = false;
	
	for (auto layer = map.GetLayers().begin(); layer != map.GetLayers().end(); ++layer)
	{
		if (layer->name == "Collision")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				if (object->Contains(character.getSprite().getPosition())) {
					character.setPosition(character.getPastPosition());
				}

				if (object->Contains(test.getSprite().getPosition())) {
					test.setPosition(test.getPastPosition());
					test.setCollision(true);
				}

				character.setCollisionBox(32, 32);
				test.setCollisionBox(32, 32);

				if (character.getCollisionBox().intersects(test.getCollisionBox())) {
					character.setPosition(character.getPastPosition());
					test.setPosition(test.getPastPosition());
				}
			}
		}
		if (layer->name == "Events")
		{
			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				
			}
		}
	}
}

/*********************************************************************
\Returns the center position of a specific tile.
Returns the center position of a tile based on the row and column provided.
\param row, column
*********************************************************************/
sf::Vector2f tile(int tile_row, int tile_column) {
	return sf::Vector2f(System::Tilesize*tile_row + System::Tilesize*.5, System::Tilesize*tile_column + System::Tilesize*.5);
}

/*********************************************************************
\Returns the center position of a specific tile.
Returns the center position of a tile based on the row and column provided.
\param row, column
*********************************************************************/
sf::Vector2f Normalize2f(sf::Vector2f vector) {
	int x = vector.x;
	int y = vector.y;
	
	if (x < 0)
		x = -1;
	else if (x > 0)
		x = 1;

	if (y < 0)
		y = -1;
	else if (x > 0)
		y = 1;
	
	return sf::Vector2f(x, y);
}