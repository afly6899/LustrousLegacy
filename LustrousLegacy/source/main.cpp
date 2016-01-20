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
#include "Textbox.h"
#include "Title.h"
#include "Pause.h"
#include "Fader.h"
#include "TutorialEvent.h"
#include "Actor.h"
#include "Character.h"
#include "sfMath.h"
using namespace std;

void sysCollision(std::vector<Actor*> actors, tmx::MapLoader& map);
void actorCollision(std::vector<Actor*>);

sf::Vector2f tile(int tile_row, int tile_column);
sf::Vector2f Normalize2f(sf::Vector2f pos);

class byDepth
{
public:
	inline bool operator() (Pawn* pawn1, Pawn* pawn2)
	{
		return (pawn1->getDepth() < pawn2->getDepth());
	}

};

int main() {

	/*********************************************************************
	GAME WINDOW PARAMETERS:
	*********************************************************************/

	sf::Vector2u window_size(800, 600);
	string window_name = "Lustrous Legacy (Prototype)";

	/*********************************************************************
	GAME WINDOW
	*********************************************************************/

	sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), window_name);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	/*********************************************************************
	GAME CAMERA AND CLOCK
	*********************************************************************/

	sf::View playerView(sf::FloatRect(0, 0, window_size.x, window_size.y));
	sf::Clock gameClock;

	/*********************************************************************
	SYSTEM PARAMETERS:
	*********************************************************************/

	sf::Font sysFont;
	if (!sysFont.loadFromFile("resources/font/Vera.ttf")) {
		cerr << "Font Error" << endl;
	}

	float elapsedTime = 0;

	/*********************************************************************
	SYSTEM SWITCHES:
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

	sf::Texture lukeTexture;
	if (!lukeTexture.loadFromFile("resources/textures/lukeSprite.png")) {
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
	
	/*********************************************************************
	SOUNDS
	*********************************************************************/

	sf::Sound sfx_blip1;
	sf::Sound sfx_blip2;

	sf::SoundBuffer bleep1;
	if (!bleep1.loadFromFile("resources/audio/text_blip.wav"))
		return -1; // error

	sf::SoundBuffer bleep2;
	if (!bleep2.loadFromFile("resources/audio/text_blip2.wav"))
		return -1; // error

	sfx_blip1.setBuffer(bleep1);
	sfx_blip2.setBuffer(bleep2);

	/*********************************************************************
	WORLD PARAMETERS:
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

	Character player(pTexture);
	player.setPosition(tile(10, 10));

	/*********************************************************************
	PREPARE TEXTBOX
	Create the textbox object and set position based on character.
	*********************************************************************/

	/*********************************************************************
	PREPARE SCREENS, ANIMATIONS, AND UTILITIES
	*********************************************************************/

	Title sysTitle(sysFont, window_size, titleTexture, sfx_blip1, music);
	Pause sysPause(sysFont, window_size);
	SceneReader* reader = new SceneReader("resources/script/scenes.txt", "Scene1");
	Fader sysFader;

	std::string scene_name = "Scene1";

	/*********************************************************************
	ENTITY LIST
	*********************************************************************/
	Actor test_actor(lukeTexture);
	test_actor.setPosition(tile(10, 15));

	std::vector<Actor*> actors;
	std::vector<Pawn*> entities;

	Actor* actorPtr;

	actorPtr = &player;
	actors.push_back(actorPtr);
	actorPtr = &test_actor;
	actors.push_back(actorPtr);
	
	for (int i = actors.size(); i != 0; i--) {
		entities.push_back(actors[i - 1]);
	}

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
				window_size.x = event.size.width;
				window_size.y = event.size.height;
				window.setSize(window_size);
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
					sysTitle.change_selection(4, Cursor_Direction::Down);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && title) {
					sysTitle.change_selection(4, Cursor_Direction::Up);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && sysTitle.getSelection() == Selection::Play_Game) {
					sysFader.resetFader();
					title = false;
					intro = false;
					if (!pause) {
						music.play();
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && title && sysTitle.getSelection() == Selection::Exit) {
					window.close();
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2) && !pause && !title)
					textbox = !textbox;
			}
		}

		if ((music.getStatus() == sf::Music::Stopped) && !title && !pause) {
			music.play();
		}

		// get the elapsed time from the game clock
		elapsedTime = gameClock.restart().asMilliseconds();

		if (!pause) {
			aniCounter += elapsedTime;
		}

		// if the game is not paused, perform normal game actions
		if (!pause && !title && window.hasFocus())
		{
			player.move(elapsedTime, player.getPlayerController().get_input());

			//test move to vector position
			test_actor.setSpeed(2);
			test_actor.move(elapsedTime, player.getPosition());

			window.setView(playerView);
			// world collision handling with quad trees
			sysCollision(actors, ml);

			// test interaction
			if (test_actor.getSprite().getGlobalBounds().intersects(player.getSprite().getGlobalBounds())) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					sf::Vector2f player_dir = sfmath::Normalize(player.getPosition() - test_actor.getPosition());
					test_actor.setDirection(sfmath::vecDirection(player_dir));
					std::cout << "IN EVENT ZONE" << std::endl;
				}
			}

			// adjust the camera to be viewing player
			playerView.setCenter(player.getPosition());
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
		
		std::sort(entities.begin(), entities.end(), byDepth());
		// draw entities
		for (int i = entities.size(); i != 0; i--) {
			window.draw(*entities[i-1]);
		}

		// draw top layer of map
		ml.Draw(window, Layer::Overlay);

		if (title) {
			window.draw(sysTitle);

		}
		if (pause)
		{
			sysPause.setPosition(playerView.getCenter());
			window.draw(sysPause);
		}

		// update screen with changes
		window.display();
	}
		return 0;
}

/*********************************************************************
/brief temp
*********************************************************************/
void actorCollision(std::vector<Actor*> actors)
{
	for (auto actor = actors.begin(); actor != actors.end(); actor++) {
		for (auto actor_check = actors.begin(); actor_check != actors.end(); actor_check++) {
			(*actor)->setCollisionBox(32, 32);
			(*actor_check)->setCollisionBox(32, 32);
			if ((*actor)->getCollisionBox().intersects((*actor_check)->getCollisionBox())) {
				if ((*actor)->getPosition() != (*actor_check)->getPosition()) {
					(*actor)->setPosition((*actor)->getPastPosition());
					(*actor_check)->setPosition((*actor_check)->getPastPosition());
				}
			}
		}
	}
}

/*********************************************************************
\brief Performs collision and event handling.
1. Determines if a player has collided with an object and returns the player to their previous position if true.
2. Determines if a player has entered an event tile and has initiated event.

\param Player, Map, Collision Switch, Player Use Switch, Player Event Switch
*********************************************************************/
void sysCollision(std::vector<Actor*> actors, tmx::MapLoader& map)
{
	// perform basic actor collision checking
	actorCollision(actors);

	// perform actor/world collision detection
	for (auto actor = actors.begin(); actor != actors.end(); actor++) {
		map.UpdateQuadTree((*actor)->getSprite().getGlobalBounds());
		std::vector<tmx::MapObject*> temp = map.QueryQuadTree((*actor)->getSprite().getGlobalBounds());
		for (auto object = temp.begin(); object != temp.end(); object++) {
			if ((*object)->GetParent() == "Collision" && (*object)->Contains((*actor)->getPosition())) {
				(*actor)->setPosition((*actor)->getPastPosition());
				break;
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
