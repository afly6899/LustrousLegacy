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
#include <ltbl/lighting/LightSystem.h>
#include "tmx/MapLoader.h"
// Class definitions
#include "SceneReader.h"
#include "Enums.h"
#include "player.h"
#include "NPC.h"
#include "textbox.h"
#include "title.h"
#include "pause.h"
#include "fader.h"
#include "TutorialEvent.h"
#include "debug/debug.h"
using namespace std;

void sysCollision(Player& player, tmx::MapLoader& map, bool& collision, bool& player_event, bool& event_2_complete);
void sysPause(bool& pause, bool& intro, bool& title, sf::Music& music, Fader& sysFader);
sf::Vector2f tile(int tile_row, int tile_column);

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
	window.setVerticalSyncEnabled(false);
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
	Player actorPlayer(pTexture);
	actorPlayer.setPosition(tile(10, 9));

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
	SceneReader* reader = new SceneReader("resources/script/scenes.txt", "Intro");
	Fader sysFader;
	Textbox* _Textbox = nullptr;
	NPC tempNPC(npcTexture);
	tempNPC.setPosition(tile(9, 9));
	NPC book(bookTexture);

	/*********************************************************************
	HARD CODED PROTOTYPE PREVIEW
	*********************************************************************/
	// Audrey's trying to do game event stuff
	bool event_start = true; // only turns true if press Enter at that start box thing
							  // once true, should have the character move down 2 spaces
							  //    int start_pos, end_pos; //once it reaches 2, stop event
							  //    Direction event_move = Direction::South;
	Event tutorial = { { Direction::South, System::Tilesize * 7} ,{ Direction::East, System::Tilesize * 3 },{ Direction::North,System::Tilesize } };
	bool fade_out = false;
	bool fade_in = false;
	bool scene1_complete = false;
	bool scene2_complete = false;
	bool eventmessage1 = false;
	bool eventmessage2 = false;
	std::string scene_name;


	/*********************************************************************
	LIGHTING SYSTEM TEST
	*********************************************************************/

	sf::Shader unshadowShader;
	sf::Shader lightOverShapeShader;
	unshadowShader.loadFromFile("resources/unshadowShader.vert", "resources/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("resources/lightOverShapeShader.vert", "resources/lightOverShapeShader.frag");

	sf::Texture penumbraTexture;
	penumbraTexture.loadFromFile("resources/penumbraTexture.png");
	penumbraTexture.setSmooth(true);
	sf::Texture pointTexture;
	pointTexture.loadFromFile("resources/pointLightTexture.png");
	pointTexture.setSmooth(true);

	ltbl::LightSystem ls;
	ls.create(sf::FloatRect(-1000.0f, -1000.0f, 1000.0f, 1000.0f), window.getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);

	std::shared_ptr<ltbl::LightPointEmission> light = std::make_shared<ltbl::LightPointEmission>();

	light->_emissionSprite.setOrigin(sf::Vector2f(pointTexture.getSize().x * 0.5f, pointTexture.getSize().y * 0.5f));
	light->_emissionSprite.setTexture(pointTexture);
	light->_emissionSprite.setScale(sf::Vector2f(20.0f, 20.0f));
	light->_emissionSprite.setColor(sf::Color(255, 230, 200));
	light->_emissionSprite.setPosition(sf::Vector2f(400, 300)); // This is where the shadows emanate from relative to the sprite

	ls.addLight(light);

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
				sysPause(pause, intro, title, music, sysFader);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
					debug = !debug;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && title) {
					screenTitle.change_selection(4, Cursor_Direction::Down);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
					_Textbox->setSpeed(0);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
					_Textbox->setSpeed(25);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
					_Textbox->setSpeed(50);
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && title) {
				screenTitle.change_selection(4, Cursor_Direction::Up);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && screenTitle.getSelection() == Selection::Play_Game) {
				sysFader.resetFader();
				actorPlayer.setPosition(tile(10, 9));
				actorPlayer.setDirection(Direction::South);
				title = false;
				intro = true;
				pause = false;
				_Textbox = new Textbox(faceMap, sysFont, soundBleep, window_width, window_height, true);
				_Textbox->setPosition(actorPlayer.getPosition());
				if (!pause) {
					music.play();
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && screenTitle.getSelection() == Selection::Exit) {
				window.close();
			}
		}
	}

		if ((music.getStatus() == sf::Music::Stopped) && !title && !pause) {
			music.play();
		}

		// START Get debug information:
		textDebug.setString("FPS: " + to_string(1 / gameClock.getElapsedTime().asSeconds()).substr(0, 5) +
			"\nCoordinates: (" + to_string(actorPlayer.getPosition().x).substr(0, 5) + ", " +
			to_string(actorPlayer.getPosition().y).substr(0, 5) + ")\nTile Map: (" +
			to_string(actorPlayer.getPosition().x / Tilesize).substr(0, 5) + ", " +
			to_string(actorPlayer.getPosition().y / Tilesize).substr(0, 5) + ")");
		// END debug information

		// LIGHTING SYSTEM TEST
		ls.render(window.getDefaultView(), unshadowShader, lightOverShapeShader);
		sf::Sprite sprite;
		sprite.setOrigin(400, 300);
		sprite.setTexture(ls.getLightingTexture());
		sf::RenderStates lightRenderStates;
		lightRenderStates.blendMode = sf::BlendMultiply;
		// END LIGHTING SYSTEM TEST

		// prime the camera
		if (!title)
		{
			playerView.setCenter(actorPlayer.getPosition());
			window.setView(playerView);
		}

		// get the elapsed time from the game clock
		elapsedTime = gameClock.restart().asMilliseconds();
		if (!pause) {
			aniCounter += elapsedTime;
		}

		if (fade_in && !sysFader.isComplete() && scene1_complete)
		{
			sysFader.setPosition(playerView.getCenter());
			sysFader.performFade(Fade::In, Speed::Slow);

		}
		else if (fade_in && sysFader.isComplete()) {

		}
		if (fade_out && !sysFader.isComplete())
		{
			sysFader.performFade(Fade::Out, Speed::Slow);
			if (sysFader.isComplete()) {
				sysFader.setPosition(playerView.getCenter());
				actorPlayer.setPosition(tile(32, 20));
				fade_out = false;
				fade_in = true;
				sysFader.resetFader();
				event_start = true;
				tutorial = Event({ {Direction::South, System::Tilesize}, { Direction::East,System::Tilesize * 3 }, { Direction::North, System::Tilesize * 1 } });
			}
		}
		// if the game is not paused, perform normal game actions
		if (!pause && !title && window.hasFocus() && sysFader.isComplete() && !eventmessage1 && !eventmessage2)
		{
			// START - PLAYER MOVEMENT (manual or automatic)
			if (event_start) {
				tutorial.runEvent(event_start, actorPlayer, elapsedTime);
				if (!event_start) {
					if (fade_in != true) {
						eventmessage1 = true;
					}
					if (scene1_complete && !scene2_complete) {
						eventmessage2 = true;
					}
				}
			}
			else {
				// START - PLAYER MOVEMENT (manual or automatic)
				actorPlayer.move(player_speed, elapsedTime, collision, move_flag);
				// END
			}
			// END 

			// START - COLLISION AND EVENT DETECTION (remove scene2_complete when redoing intro)
			sysCollision(actorPlayer, ml, collision, player_event, scene2_complete);
			// END 

			// adjust the camera to be viewing player
			playerView.setCenter(actorPlayer.getPosition());
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

		if (eventmessage1 || eventmessage2)
			actorPlayer.idle();
			
		// draw player
		window.draw(actorPlayer);

		// draw top layer of map
		ml.Draw(window, Layer::Overlay);

		// LIGHTING TEST
		sprite.setPosition(actorPlayer.getPosition());
		window.draw(sprite, lightRenderStates);
		// LIGHTING TEST

		if (fade_in)
		{
			window.draw(sysFader);
		}

		if (fade_out) {
			sysFader.setPosition(actorPlayer.getPosition());
			window.draw(sysFader);
		}

		if (title) {
			screenTitle.animate(elapsedTime);
			window.draw(screenTitle);

		}
		else if (!title && !intro && event_start) {
			sysFader.setPosition(playerView.getCenter());
			sysFader.performFade(Fade::In, Speed::Slow);
			window.draw(sysFader);
		}
		else if (intro || (eventmessage1) || (eventmessage2))
		{
			_Textbox->setPosition(playerView.getCenter());
			if (intro) {
				book.setPosition(playerView.getCenter());
				sysFader.setPosition(playerView.getCenter());
				window.draw(sysFader.blackScreen());
				book.hover(elapsedTime);
				window.draw(book);
			}
			if (!pause && window.hasFocus()) {
				if (!_Textbox->if_endMessage())
					_Textbox->message(reader->currentMessage().second, reader->currentMessage().first, elapsedTime);
				else
				{
					_Textbox->reset();
					if (!reader->isEmpty())
						reader->nextMessage();
					if (reader->isEmpty()) {
						if (!intro) {
							scene1_complete = true;
						}
						if (scene1_complete && eventmessage1) {
							sysFader.resetFader();
							fade_out = true;
							eventmessage1 = false;
						}
						if (scene1_complete && eventmessage2) {
							eventmessage2 = false;
							scene2_complete = true;
						}
						if (intro)
							scene_name = "Scene1";
						if (scene1_complete)
							scene_name = "TalkingToLuke";
						intro = false;
						pause = false;
						delete _Textbox;
						_Textbox = new Textbox(faceMap, sysFont, soundBleep, window_width, window_height);
						delete reader;
						reader = new SceneReader("resources/script/scenes.txt", scene_name);
					}
				}
			}
		}

		if (intro || (!fade_out && sysFader.isComplete()))
			window.draw(*_Textbox);
		// END HARD-CODED ALPHA PREVIEW

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
void sysCollision(Player& player, tmx::MapLoader& map, bool& collision, bool& player_event, bool& event2done)
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
				if ((object->GetName() == "Luke1") && player.getDirection() == Direction::East)
				{
					if (sf::Vector2f(object->GetPosition().x + 32, object->GetPosition().y + 32) == player.getPosition())
						player_event = true;
					else
						player_event = false;
				}
				else if ((object->GetName() == "Luke2") && player.getDirection() == Direction::North && event2done)
				{
					if (sf::Vector2f(object->GetPosition().x + 32, object->GetPosition().y + 32) == player.getPosition())
						player_event = true;
					else
						player_event = false;
				}
			}
		}
	}
}

/*********************************************************************
\brief Determines if player has pressed esc.
Checks if the player wishes to pause the game. The game will pause if 'esc' is pressed and the game is not paused. 
The game will resume if 'esc' is pressed and the game is paused.
\param Pause Switch, Intro Switch, Title Switch, Music, Fader
*********************************************************************/

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

/*********************************************************************
\Returns the center position of a specific tile.
Returns the center position of a tile based on the row and column provided.
\param row, column
*********************************************************************/
sf::Vector2f tile(int tile_row, int tile_column) {
	return sf::Vector2f(System::Tilesize*tile_row + System::Tilesize*.5, System::Tilesize*tile_column + System::Tilesize*.5);
}