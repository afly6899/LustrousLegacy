/*********************************************************************
Game: Lustrous Legacy
UC Irvine - Winter 2016 Quarter (VGDC Project)

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
#include "MoveStep.h"
#include "Step.h"

using namespace std;

// main functions temporarily located here

void sysCollision(std::vector<Actor*>& actors, tmx::MapLoader& map);
void actorCollision(std::vector<Actor*>&);
bool UI_visible(std::vector<UI*>& sysWindows);
bool UI_visible_excluding(UI* sysWindow, std::vector<UI*> sysWindows);

sf::Vector2f tile(int tile_row, int tile_column);

// used to sort a vector of pawns by depth
class byDepth
{
public:
	inline bool operator() (Pawn* pawn1, Pawn* pawn2)
	{
		return (pawn1->getDepth() < pawn2->getDepth());
	}

};

// end main functions

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

	bool intro = false;
	bool textbox = false;
	bool ENTER_KEY = false;

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
	if (!titleTexture.loadFromFile("resources/textures/cursor.png")) {
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
	playerView.setCenter(player.getViewArm());

	/*********************************************************************
	OLD PROTOTYPE STUFF THAT HAS NOT BEEN REMOVED YET
	*********************************************************************/

	Fader sysFader;
	std::string scene_name = "Scene1";

	/*********************************************************************
	UI PROCESSING
	*********************************************************************/

	std::vector<UI*> sysWindows;

	Title* titlePtr = new Title(sysFont, window_size, titleTexture, sfx_blip1, music);
	Pause* pausePtr = new Pause(sysFont, window_size);

	titlePtr->setVisible(true);

	sysWindows.push_back(titlePtr);
	sysWindows.push_back(pausePtr);

	/*********************************************************************
	ENTITY PROCESSING
	*********************************************************************/
	Actor test_actor(lukeTexture);
	test_actor.setPosition(tile(10, 15));
	test_actor.addTargetPosition(tile(12, 12));
	test_actor.addTargetPosition(tile(12, 10));
	test_actor.addTargetPosition(tile(10, 10));
	test_actor.addTargetPosition(tile(10, 12));


	// EVENT TESTING //
	std::vector<sf::Vector2f> positions;
	positions.push_back(tile(12, 12));
	positions.push_back(tile(12, 10));
	positions.push_back(tile(10, 10));
	positions.push_back(tile(10, 12));

	MoveStep* move_ptr = new MoveStep(positions);
	std::queue<Step*> step_ptrs;
	step_ptrs.push(move_ptr);

	//

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

	Textbox* _Textbox = new Textbox(faceMap, sysFont, sfx_blip1, sf::Vector2f(window_size.x, window_size.y));
	
	
	// TEST //

		
	// END TEST //
	/*********************************************************************
	BEGIN GAME LOOP:
	*********************************************************************/

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case(sf::Event::Closed) :
				window.close();
				break;
			case(sf::Event::Resized) :
				window_size = sf::Vector2u(event.size.width, event.size.height);
				window.setSize(window_size);
				break;
			case(sf::Event::KeyPressed) :					
				if (titlePtr->isVisible()) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
						titlePtr->change_selection(Cursor_Direction::Down);
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
						titlePtr->change_selection(Cursor_Direction::Up);
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
						switch (titlePtr->getSelection()) {
						case(Selection::Play_Game) :
							sysFader.resetFader();
							titlePtr->setVisible(false);
							intro = true;
							break;
						case(Selection::Exit) :
							window.close();
							break;
						case(Selection::Load_Game) :
							std::cout << "There is no implementation of this option yet." << std::endl;
						case(Selection::Settings) :
							std::cout << "There is no implementation of this option yet." << std::endl;
						}
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					if (!UI_visible_excluding(pausePtr, sysWindows)) {
						pausePtr->setVisible(!pausePtr->isVisible());
						if (pausePtr->isVisible())
							music.pause();
						else
							music.play();
					}
				}
				break;
			}
		}

		// get the elapsed time from the game clock
		elapsedTime = gameClock.restart().asMilliseconds();

		// if there is no UI visible, perform normal game actions!
		if (!UI_visible(sysWindows) && window.hasFocus() && !textbox)
		{
			aniCounter += elapsedTime;
			if (music.getStatus() == sf::Music::Stopped)
				music.play();

			player.move(elapsedTime, player.controller.get_input());

			//test move to vector position
			test_actor.setSpeed(2);
			//test_actor.cycleMovement(elapsedTime);
			/*else
				test_actor.setPosition(tile(10, 15));*/
			// end test move for test_actor

			// world collision handling with quad trees
			sysCollision(actors, ml);

			// test interaction
			/*if (test_actor.getSprite().getGlobalBounds().intersects(player.getSprite().getGlobalBounds())) {

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
					unsigned int dir_player_from_actor = sfmath::vecTrueDirection(sfmath::Normalize(player.getPosition() - test_actor.getPosition()));
					std::cout << dir_player_from_actor << "(from test_actor)<------>(player facing)" << player.getDirection() << endl;
					std::cout << player.getDirection() << std::endl;

					if (dir_player_from_actor != player.getDirection()) {
						// alot of the direction system needs to change for this to work properly
						test_actor.faceActor(player);
						test_actor.collided();
						if (!textbox) {
							textbox = !textbox;
						}
					}
				}
			}
			*/

			if (step_ptrs.front()->run(elapsedTime, test_actor)) {
				std::cout << "doing" << endl;
			}

			// end test interaction

			// adjust the camera to be viewing player
			playerView.setCenter(player.getViewArm());
		}

		// end

		//

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
	
		// draw UI
		for (auto sys = sysWindows.begin(); sys != sysWindows.end(); sys++) {
			if ((*sys)->isVisible()) {
				(*sys)->update(player.getViewArm(), elapsedTime);
				window.draw(**sys);
			}
		}

		if (textbox) {
			std::string message[] = { "resources/script/scenes.txt", "LukeTEST" };
			if (textbox) {
				player.resetTextureRect();
				textbox = _Textbox->display_message(message, player, elapsedTime);
				if (textbox)
					window.draw(*_Textbox);
			}
		}

		// update screen with changes
		window.display();
	}
		return 0;
}

/*********************************************************************
/brief temp
*********************************************************************/
void actorCollision(std::vector<Actor*>& actors)
{
	for (auto actor = actors.begin(); actor != actors.end(); actor++) {
		for (auto actor_check = actors.begin(); actor_check != actors.end(); actor_check++) {
			if ((*actor)->getCollisionBox().intersects((*actor_check)->getCollisionBox())) {
				if ((*actor)->getPosition() != (*actor_check)->getPosition()) {
					(*actor)->collided();
					(*actor_check)->collided();
				}
			}
		}
	}
}

/*********************************************************************
\brief Performs collision and event handling.
*********************************************************************/
void sysCollision(std::vector<Actor*>& actors, tmx::MapLoader& map)
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


/*********************************************************************
\brief temp
*********************************************************************/
bool UI_visible(std::vector<UI*>& sysWindows) {
	for (auto sys = sysWindows.begin(); sys != sysWindows.end(); sys++) {
		if ((*sys)->isVisible()) {
			return true;
		}
	}
	return false;
}

/*********************************************************************
\brief temp
*********************************************************************/
bool UI_visible_excluding(UI* sysWindow, std::vector<UI*> sysWindows) {
	sysWindows.erase(std::find(sysWindows.begin(), sysWindows.end(), sysWindow));
	return UI_visible(sysWindows);
}
