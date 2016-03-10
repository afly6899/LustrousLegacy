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

//// Include libraries
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <math.h>
#include "tmx/MapLoader.h"

//// Class definitions
#include "SceneReader.h"
#include "Enums.h"
#include "Textbox.h"
#include "Title.h"
#include "Pause.h"
#include "Fader.h"
#include "Actor.h"
#include "Character.h"
#include "sfMath.h"
#include "Step.h"

// Audrey Edit: Adding Event class functionalities //
#include "Event.h"
#include "DirectionStep.h"
#include "MoveStep.h"
#include "SpeechStep.h"
#include "TogetherStep.h"
// *************** End Audrey Edit *************** //

using namespace std;

// MAIN FUNCTIONS
void sysCollision(std::vector<Actor*>& actors, tmx::MapLoader& map);
void actorCollision(std::vector<Actor*>&);
bool UI_visible(std::vector<UI*>& sysWindows);
bool UI_visible_excluding(UI* sysWindow, std::vector<UI*> sysWindows);
//void load_map(tmx::MapLoader& ml, std::string map_name, Character& player, std::vector<Actor*>& actors, std::vector<Pawn*>& pawns, std::map<std::string, sf::Texture*> texMap); // original
void load_map(tmx::MapLoader& ml, std::string map_name, Character& player, std::vector<Actor*>& actors, std::vector<Pawn*>& pawns, std::map<std::string, sf::Texture*> texMap, Event& events);
void animateMap(tmx::MapLoader& ml, sf::RenderWindow& window, float(&worldAnimationArr)[3]);
void drawTextbox(sf::RenderWindow& window, Textbox* textbox, bool flag);
void drawEntities(sf::RenderWindow& window, std::vector<Pawn*>& entities);
void drawUI(sf::RenderWindow& window, sf::View playerView, std::vector<UI*>& sysWindows, float elapsedTime);
sf::Vector2f Vector2iTo2f(sf::Vector2i pos);
sf::Vector2f tile(int tile_row, int tile_column);

// TEMP -> DEPTH SORT CLASS
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
	playerView.setCenter(Vector2iTo2f(window.getPosition()));
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

	bool initial_load_map = false;
	bool textbox = false;

	// Audrey Edit: Adding Event class functionalities //
	bool eventIsRunning = false;
	// *************** End Audrey Edit *************** //

	/*********************************************************************
	TEXTURES
	*********************************************************************/

	// WARREN SPRITE
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("resources/textures/playerSprite.png")) {
		cerr << "Texture Error" << endl;
	}

	// LUKE SPRITE
	sf::Texture lukeTexture;
	if (!lukeTexture.loadFromFile("resources/textures/lukeSprite.png")) {
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

	// textureMap HOLDS THE REFERENCES OF ALL TEXTURES
	std::map<std::string, sf::Texture*> textureMap;
	textureMap["Warren"] = &pTexture;
	textureMap["Luke"] = &lukeTexture;

	// faceMap HOLDS ALL THE FACE SPRITES
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
	WORLD ANIMATION PARAMETERS:
	*********************************************************************/
	float worldAnimationArr[] = { 0, 800, 0 };

	/*********************************************************************
	PREPARE MAP
	*********************************************************************/

	tmx::MapLoader ml("resources/maps");
	std::string map_name = "start.tmx";
	std::string current_map = "";

	/*********************************************************************
	PREPARE CHARACTER
	*********************************************************************/

	Character player(pTexture);

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
	titlePtr->setPosition(Vector2iTo2f(window.getPosition()));

	sysWindows.push_back(titlePtr);
	sysWindows.push_back(pausePtr);

	/*********************************************************************
	ENTITY PROCESSING
	*********************************************************************/

	std::vector<Actor*> actors;
	std::vector<Pawn*> entities;
	actors.push_back(&player);

	/*********************************************************************
	TEXT RENDERING
	*********************************************************************/

	Textbox* _Textbox = new Textbox(faceMap, sysFont, sfx_blip1, sf::Vector2f(window_size.x, window_size.y));
	std::string message[] = { "resources/script/scenes.txt", "Intro" };



	// Audrey Edit: Adding Event class functionalities //
	//Actor* test_luke = new Actor(lukeTexture);
	//test_luke->setScene(scene_name);
	//test_luke->setPosition(tile(11,10));
	//test_luke->setPastPosition(tile(11,10));
	//test_luke->setDirection(Direction::South);
	//actors.push_back(test_luke);

	//Event test_events({ new TogetherStep(std::vector<sf::Vector2f>({ tile(10,15), tile(11,15) }), std::vector<Actor*>({ &player, test_luke })) }, { &player});

	Event test_events;
	
	// *************** End Audrey Edit *************** //

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
							initial_load_map = true;
							break;
						case(Selection::Exit) :
							window.close();
							break;
						case(Selection::Load_Game) :
							std::cout << "There is no implementation of this option yet." << std::endl;
							break;
						case(Selection::Settings) :
							std::cout << "There is no implementation of this option yet." << std::endl;
							break;
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
				// Audrey Edit: Adding Event class functionalities //
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
					if (!textbox && !pausePtr->isVisible()) {
				
						if (!test_events.finishedEvents())
							eventIsRunning = test_events.startEvent();
					}
				}
				// *************** End Audrey Edit *************** //
				break;
			}
		}

		elapsedTime = gameClock.restart().asMilliseconds();

		if (current_map != map_name && initial_load_map)
		{
			load_map(ml, map_name, player, actors, entities, textureMap, test_events);
			for (int i = actors.size(); i != 0; i--) {
				entities.push_back(actors[i - 1]);
			}
			current_map = map_name;
			// Audrey Edit: Adding Event class functionalities //
			//test_events.addEvents(new DirectionStep(Direction::West), actors[2]);
			//test_events.addEvents(new DirectionStep(Direction::East), &player);
			//test_events.addEvents(new SpeechStep(message[1], textbox), actors[2]);
			// *************** End Audrey Edit *************** //
		}

		// if there is no UI visible, perform normal game actions!
		if (!UI_visible(sysWindows) && window.hasFocus())
		{
			worldAnimationArr[Map::Counter] += elapsedTime;
			if (music.getStatus() == sf::Music::Stopped)
				music.play();

			if (!textbox) {
				// Audrey Edit: Adding Event class functionalities //
				if (eventIsRunning) {
					if (test_events.getEventType() == "Speech" && !textbox) {
						textbox = eventIsRunning;
					}
					std::cout << "Doing Event Number: " << test_events.getEventType() << std::endl;
					test_events.runEvent(elapsedTime);
					eventIsRunning = test_events.eventIsRunning();
					if (textbox) {
						textbox = eventIsRunning;
					}
				}
				else {
					// *************** End Audrey Edit *************** //
					player.move(elapsedTime, player.controller.get_input());
					// Audrey Edit: Adding Event class functionalities //
					//if (player.getPosition().y >= System::Tilesize * 15 && player.getPosition().y <= System::Tilesize * 15 + System::Tilesize*.5) {
						//eventIsRunning = test_events.startEvent();
					//}
					// *************** End Audrey Edit *************** //
					sysCollision(actors, ml);

					// TEST INTERACTION BETWEEN PLAYER AND OTHER ACTORS
					for (auto actor = actors.begin(); actor != actors.end(); actor++)
					{
						if ((*actor)->getClass() != "Character") {
							textbox = player.check_Interact(**actor);
							if (textbox) {
								message[1] = (*actor)->getScene();
								break;
							}
						}
					}
					// Audrey Edit: Adding Event class functionalities //
				}
				// *************** End Audrey Edit *************** //
				playerView.setCenter(player.getViewArm());
			}
		}

		// BEGIN DRAW CYCLE

		window.clear();
		window.setView(playerView);

		if (!titlePtr->isVisible()) {

			animateMap(ml, window, worldAnimationArr);
			drawEntities(window, entities);
			ml.Draw(window, Layer::Overlay);

			if (textbox && !UI_visible(sysWindows)) {
				textbox = _Textbox->display_message(message, player, elapsedTime);
			}
		}

		drawTextbox(window, _Textbox, textbox);
		drawUI(window, playerView, sysWindows, elapsedTime);

		window.display();
	}
	return 0;
}

/*********************************************************************
/brief Performs the collision between actors.
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
\brief Performs the collision handling for all actors.
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
\brief Returns the center position of a specific tile.
*********************************************************************/
sf::Vector2f tile(int tile_row, int tile_column) {
	// Audrey Edit: Adding Event class functionalities //
	// changed the coefficient for the y tile thing to be .75
	return sf::Vector2f(System::Tilesize*tile_row + System::Tilesize*.5, System::Tilesize*tile_column + System::Tilesize*.75);
	// *************** End Audrey Edit *************** //
	//return sf::Vector2f(System::Tilesize*tile_row + System::Tilesize*.5, System::Tilesize*tile_column + System::Tilesize*.5);
}

/*********************************************************************
\brief Checks if any of the UI is visible.
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
\brief Checks if any of the UI is visible excluding the passed in UI object.
*********************************************************************/
bool UI_visible_excluding(UI* sysWindow, std::vector<UI*> sysWindows) {
	sysWindows.erase(std::find(sysWindows.begin(), sysWindows.end(), sysWindow));
	return UI_visible(sysWindows);
}

/*********************************************************************
\brief Returns the direction of the actor in a map during loading.
*********************************************************************/
int  _directionOfActor(std::string dir) {
	if (dir == "North")
		return Direction::North;
	else if (dir == "South")
		return Direction::South;
	else if (dir == "East")
		return Direction::East;
	else if (dir == "West")
		return Direction::West;
}

/*********************************************************************
\brief Loads the specified map and instantiates all actors and pawns.
	   The player is set at the start position specified by the map.
*********************************************************************/
//void load_map(tmx::MapLoader& ml, std::string map_name, Character& player, std::vector<Actor*>& actors, std::vector<Pawn*>& pawns, std::map<std::string, sf::Texture*> textureMap) {
bool sort_by_step_num(std::pair<Step*, Actor*> first, std::pair<Step*, Actor*> second) { return true; }
void load_map(tmx::MapLoader& ml, std::string map_name, Character& player, std::vector<Actor*>& actors, std::vector<Pawn*>& pawns, std::map<std::string, sf::Texture*> textureMap, Event& events) {
	ml.Load(map_name);
	std::vector<std::pair<Step*, Actor*>> steps;

	for (auto layer = ml.GetLayers().begin(); layer != ml.GetLayers().end(); ++layer)
	{
		if (layer->name == "Setup")
		{
			for (int i = 0; i < std::stoi(layer->properties["numEvents"]); i++) {
				steps.push_back({ nullptr, nullptr });
			}
			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				if (object->GetName() == "START")
					player.setPosition(object->GetCentre());
				else if (object->GetName() == "ACTOR")
				{
					std::string scene_name = object->GetPropertyString("Scene");
					Actor* ptr = new Actor(*textureMap[object->GetPropertyString("Texture")]);
					ptr->setScene(scene_name);
					ptr->setPosition(object->GetCentre());
					ptr->setPastPosition(ptr->getPosition());
					ptr->setDirection(_directionOfActor(object->GetPropertyString("Direction")));
					actors.push_back(ptr);
				}
				else if (object->GetName() == "PAWN")
				{
					Pawn* ptr = new Pawn(*textureMap[object->GetPropertyString("Texture")]);
					pawns.push_back(ptr);
				}
				// Adding
				std::string order = object->GetPropertyString("Event Order");
				std::cout << object->GetPropertyString("ID") << std::endl;
				if (object->GetPropertyString("ID") == "108") {
					std::cout << "Switching using " << order << std::endl;
				}
				if (order != "") {
					for (int i = 0; i < order.size(); i++)
					{
						std::string e = object->GetPropertyString(std::string(1, order[i]));

						Actor* moving_actor = (object->GetName() == "START") ? actors[0] : actors[actors.size() - 1];
						Step* step = nullptr;
						switch (e[0] - '0') {
						case 0:
							step = new MoveStep(std::vector<sf::Vector2f>({ tile(std::stoi(e.substr(1, 2)), std::stoi(e.substr(4, 2))) }));
							break;
						case 1:
							if (steps[order[i] - '0' - 1].first == nullptr) {
								step = new TogetherStep(std::vector<sf::Vector2f>({ tile(std::stoi(e.substr(1, 2)), std::stoi(e.substr(4, 2))) }), std::vector<Actor*>({ moving_actor }));
							}
							else {
								std::cout << object->GetPropertyString("ID") << std::endl;
								steps[order[i] - '0' - 1].first->addToStep(sf::Vector2f({ tile(std::stoi(e.substr(1, 2)), std::stoi(e.substr(4, 2))) }), moving_actor);
							}
							break;
						case 2:
							break;
						}
						if (step != nullptr) {
							steps[order[i] - '0' - 1] = (std::pair<Step*, Actor*>(step, moving_actor));
						}
					}

				}
			}
		}
	}
	for (auto step : steps) {
		events.addEvents(step.first, step.second);
		std::cout << step.first->getType() << " ";
	}
	std::cout << std::endl;
}

/*********************************************************************
\brief Animates the background of maps.
*********************************************************************/
void animateMap(tmx::MapLoader& ml, sf::RenderWindow& window, float(&worldAnimationArr)[3]) {
	if (worldAnimationArr[Map::Counter] >= worldAnimationArr[Map::FrameDuration])
	{
		worldAnimationArr[Map::Counter] -= worldAnimationArr[Map::FrameDuration];
		ml.Draw(window, worldAnimationArr[Map::Layer]);
		worldAnimationArr[Map::Layer] += 1;
		if (worldAnimationArr[Map::Layer] > Layer::Background_2)
			worldAnimationArr[Map::Layer] = Layer::Background_1;
	}
	else
	{
		ml.Draw(window, worldAnimationArr[Map::Layer], Background_1);
	}

	// draw walkable and collidable tiles
	ml.Draw(window, Layer::Field);
	ml.Draw(window, Layer::Collision_Objects);
}

/*********************************************************************
\brief Converts sf::Vector2i to sf::Vector2f
*********************************************************************/
sf::Vector2f Vector2iTo2f(sf::Vector2i pos) {
	return sf::Vector2f(pos.x, pos.y);
}

/*********************************************************************
\brief Draws all UI elements.
*********************************************************************/
void drawUI(sf::RenderWindow& window, sf::View playerView, std::vector<UI*>& sysWindows, float elapsedTime) {
	for (auto sys = sysWindows.begin(); sys != sysWindows.end(); sys++) {
		if ((*sys)->isVisible()) {
			(*sys)->update(playerView.getCenter(), elapsedTime);
			window.draw(**sys);
		}
	}
}

/*********************************************************************
\brief Draws textbox if flag is set.
*********************************************************************/
void drawTextbox(sf::RenderWindow& window, Textbox* textbox, bool flag) {
	if (flag)
		window.draw(*textbox);
}

/*********************************************************************
\brief Draws all entities that are in the entities vector.
*********************************************************************/
void drawEntities(sf::RenderWindow& window, std::vector<Pawn*>& entities) {
	std::sort(entities.begin(), entities.end(), byDepth());
	for (int i = entities.size(); i != 0; i--) {
		window.draw(*entities[i - 1]);
	}
}

// left-overs
/*
if (step_ptrs.front()->run(elapsedTime, test_actor)) {
if (!step_ptrs.front()->run(elapsedTime,test_actor)) {
if (!step_ptrs.empty()) {
// need to manage memory
step_ptrs.pop();
}
}
}
*/