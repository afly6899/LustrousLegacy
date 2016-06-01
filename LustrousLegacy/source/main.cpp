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
#include "MapStep.h"
#include "Event.h"
#include "DirectionStep.h"
#include "MoveStep.h"
#include "SpeechStep.h"
#include "TogetherStep.h"
#include "BattleStep.h"
// *************** End Audrey Edit *************** //

// Audrey Edit: Adding Battle Functionalities
#include "FightingPawn.h"
#include "BattleScene.h"
// ++++++++++++++++++ End Audrey Edit

struct EventTile
{
	sf::Vector2f corner;
	std::string type;
	std::string info;
};


using namespace std;

// MAIN FUNCTIONS
void sysCollision(std::vector<Actor*>& actors, tmx::MapLoader& map);
void actorCollision(std::vector<Actor*>&);
bool UI_visible(std::vector<UI*>& sysWindows);
std::string onEventTile(Character player, std::vector<EventTile> event_tiles, std::string lookingFor);
bool UI_visible_excluding(UI* sysWindow, std::vector<UI*> sysWindows);
void load_map(tmx::MapLoader& ml, std::string &map_name, Character& player, std::vector<Actor*>& actors, std::map<std::string, sf::Texture*> texMap, std::vector<EventTile>& event_tiles, std::map<std::string, Event>& events, bool& textbox, string messages[], std::string &music_name, std::vector<std::string> previous_maps, BattleScene &battleScene);
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
#pragma region SettingUp
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
#pragma region BattleBackgrounds
	sf::Image forestBattleBG;
	if (!forestBattleBG.loadFromFile("resources/textures/bg_forest.png")) {
		cerr << "Image Error: bg_forest.png" << endl;
	}

	sf::Image trainingBattleBG;
	if (!trainingBattleBG.loadFromFile("resources/textures/bg_training.png")) {
		cerr << "Image Error: bg_training.png" << endl;
	}
#pragma endregion

#pragma region BattleTextures
	sf::Texture warrenBattle1;
	if (!warrenBattle1.loadFromFile("resources/textures/battleSprite_warren1.png")) {
		cerr << "Image Error: battleSpriteWarren1.png" << endl;
	}
	sf::Texture warrenBattle2;
	if (!warrenBattle2.loadFromFile("resources/textures/battleSprite_warren2.png")) {
		cerr << "Image Error: battleSpriteWarren2.png" << endl;
	}
#pragma endregion

#pragma region Player_Textures
	// WARREN SPRITE
	sf::Texture pTexture;
	if (!pTexture.loadFromFile("resources/textures/playerSprite.png")) {
		cerr << "Texture Error: playerSprite.png" << endl;
	}

	// LUKE SPRITE
	sf::Texture lukeTexture;
	if (!lukeTexture.loadFromFile("resources/textures/lukeSprite.png")) {
		cerr << "Texture Error: lukeSprite.png" << endl;
	}

	// RESDIN SPRITE
	sf::Texture resdinTexture;
	if (!resdinTexture.loadFromFile("resources/textures/resdinSprite.png")) {
		cerr << "Texture Error: sprite_resdin.png" << endl;
	}

	sf::Texture villagerTexture;
	if (!villagerTexture.loadFromFile("resources/textures/villagerSprite.png")) {
		cerr << "Texture Error: sprite_villager.png" << endl;
	}

	// WARREN FACE TEXTURE
	sf::Texture pfTexture;
	if (!pfTexture.loadFromFile("resources/textures/face_warren.png")) {
		cerr << "Texture Error: face_warren.png" << endl;
	}

	// RESDIN FACE TEXTURE
	sf::Texture resdinfTexture;
	if (!resdinfTexture.loadFromFile("resources/textures/face_resdin.png")) {
		cerr << "Texture Error: face_resdin.png" << endl;
	}

	// LUKE FACE TEXTURE
	sf::Texture lukefTexture;
	if (!lukefTexture.loadFromFile("resources/textures/face_luke.png")) {
		cerr << "Texture Error: face_luke.png" << endl;
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

	// TRANSPARENT TEXTURE
	sf::Texture transparentTexture;
	if (!transparentTexture.loadFromFile("resources/textures/transparent.png")) {
		cerr << "Texture Error: transparet.png" << endl;
	}

	// BOOK TEXTURE
	sf::Texture bookTexture;
	if (!bookTexture.loadFromFile("resources/textures/book.png")) {
		cerr << "Texture Error: book.png" << endl;
	}

	// textureMap HOLDS THE REFERENCES OF ALL TEXTURES
	std::map<std::string, sf::Texture*> textureMap;
	textureMap["Warren"] = &pTexture;
	textureMap["Luke"] = &lukeTexture;
	textureMap["Resdin"] = &resdinTexture;
	textureMap["Villager"] = &villagerTexture;
	textureMap["Empty"] = &transparentTexture;
	textureMap["Book"] = &bookTexture;


	// faceMap HOLDS ALL THE FACE SPRITES
	std::map<std::string, sf::Sprite> faceMap;
	faceMap["Warren"] = sf::Sprite(pfTexture);
	faceMap["Warren"].setOrigin(faceMap["Warren"].getLocalBounds().width*.5, faceMap["Warren"].getLocalBounds().height*.5);
	faceMap["Resdin"] = sf::Sprite(resdinfTexture);
	faceMap["Resdin"].setOrigin(faceMap["Resdin"].getLocalBounds().width*.5, faceMap["Resdin"].getLocalBounds().height*.5);
	faceMap["Luke"] = sf::Sprite(lukefTexture);
	faceMap["Luke"].setOrigin(faceMap["Luke"].getLocalBounds().width*.5, faceMap["Luke"].getLocalBounds().height*.5);
#pragma endregion

#pragma region	Sounds
	/*********************************************************************
	MUSIC
	*********************************************************************/

	std::string musicName = "LL_Title.wav", previousMusicName = "";
	sf::Music music;
	if (!music.openFromFile("resources/audio/" + musicName))
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

#pragma endregion

	/*********************************************************************
	WORLD ANIMATION PARAMETERS:
	*********************************************************************/
	float worldAnimationArr[] = { 0, 800, 0 };

	/*********************************************************************
	PREPARE MAP
	*********************************************************************/

	tmx::MapLoader ml("resources/maps");
	std::string map_name = "title.tmx";
	std::string current_map = "";
	std::vector<std::string> previous_maps;

	/*********************************************************************
	PREPARE CHARACTER
	*********************************************************************/

	Character player(pTexture);

	/*********************************************************************
	OLD PROTOTYPE STUFF THAT HAS NOT BEEN REMOVED YET
	*********************************************************************/

	Fader sysFader;
	bool start = false;
	bool canPoll = true;
	bool fade_in = false;
	bool fade_out = false;
	std::string scene_name = "Scene1";

	/*********************************************************************
	UI PROCESSING
	*********************************************************************/

	std::vector<UI*> sysWindows;

	Title* titlePtr = new Title(sysFont, window_size, titleTexture, cursorTexture, sfx_blip1, music);
	Pause* pausePtr = new Pause(sysFont, window_size);

	titlePtr->setVisible(true);
	titlePtr->setPosition(Vector2iTo2f(window.getPosition()));

	sysWindows.push_back(titlePtr);
	sysWindows.push_back(pausePtr);

	map<int, bool> ui_kb;
	ui_kb[sf::Keyboard::Return] = false;
	ui_kb[sf::Keyboard::Left] = false;
	ui_kb[sf::Keyboard::Right] = false;
	ui_kb[sf::Keyboard::Escape] = false;
	ui_kb[sf::Keyboard::Up] = false;
	ui_kb[sf::Keyboard::Down] = false;

	/*********************************************************************
	ENTITY PROCESSING
	*********************************************************************/

	std::vector<Actor*> actors;
	std::vector<Pawn*> entities;

	/*********************************************************************
	TEXT RENDERING
	*********************************************************************/

	Textbox* _Textbox = new Textbox(faceMap, sysFont, sfx_blip1, sf::Vector2f(window_size.x, window_size.y));
	std::string message[] = { "resources/script/scenes.txt", "Intro" };

	std::string currentEvent = "Story";
	std::map<std::string, Event> events;
	std::vector<EventTile> event_tiles;

	sf::Texture battleTexture;
	if (!battleTexture.loadFromFile("resources/textures/battleSprite_warren.png")) {
		cerr << "Texture Error" << endl;
	}

	sf::Texture battleBackground;
	if (!battleBackground.loadFromFile("resources/textures/bg_training.png")) {
		cerr << "Texture Error: bg_traning" << endl;
	}

	sf::Texture battleMenuTexture;
	if (!battleMenuTexture.loadFromFile("resources/textures/battleMenuBG.png")) {
		cerr << "Texture Error: battleMenuBG" << endl;
	}

	StatPawn playerStats = { 250, 15, 5, "Warren" };

	BattleScene test(battleBackground, battleTexture, playerStats);

	bool forceBattle = false;
	bool inBattle = false;
	bool endBattle = false;
	bool stopLooking = false;

	// *************** End Audrey Edit *************** //
#pragma endregion
	/*********************************************************************
	BEGIN GAME LOOP:
	*********************************************************************/

	while (window.isOpen()) {
		if (events["Story"].getPriority() && sysFader.isComplete()) {
			if (!textbox && !pausePtr->isVisible()) {
				if (!events["Story"].finishedEvents())
					eventIsRunning = events["Story"].startEvent();
			}
		}
		sf::Event event;
		while (canPoll && window.pollEvent(event)) {
			switch (event.type) {
			case(sf::Event::Closed) :
				window.close();
				break;
			case(sf::Event::Resized) :
				window_size = sf::Vector2u(event.size.width, event.size.height);
				window.setSize(window_size);
				break;
			case (sf::Event::KeyReleased) :
				ui_kb[event.key.code] = true;
				break;
			case (sf::Event::KeyPressed) :
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && !inBattle) {
					test.loadBattle(inBattle, sysFont, battleMenuTexture, cursorTexture, sfx_blip1);
					fade_out = true;
					sysFader.resetFader();
				}
			}
		}

#pragma region UI_Keyboard
		if (titlePtr->isVisible()) {
			if (ui_kb[sf::Keyboard::Up] || ui_kb[sf::Keyboard::Down])
				titlePtr->change_selection(event.key.code);
			else if (ui_kb[sf::Keyboard::Return]) {
				switch (titlePtr->getSelection()) {
				case(Selection::Play_Game) :
					sysFader.resetFader();
					start = true;
					initial_load_map = true;
					canPoll = false;
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
		else if (ui_kb[sf::Keyboard::Escape]) {
			if (!UI_visible_excluding(pausePtr, sysWindows)) {
				pausePtr->setVisible(!pausePtr->isVisible());
				if (pausePtr->isVisible())
					music.pause();
				else
					music.play();
			}
		}
		else if (!textbox && ui_kb[sf::Keyboard::Return]) {
			std::string new_map = onEventTile(player, event_tiles, "map");
			if (new_map != "") {
				map_name = new_map;
				initial_load_map = true;
			}
		}
#pragma endregion
		if (!eventIsRunning) {
			std::string possibleEvent = onEventTile(player, event_tiles, "trigger");
			if (!stopLooking && possibleEvent != "") {
				currentEvent = possibleEvent;
				eventIsRunning = events[currentEvent].startEvent();
				stopLooking = true;
			}
			else if (stopLooking && possibleEvent == "") {
				stopLooking = false;
			}
		}
		elapsedTime = gameClock.restart().asMilliseconds();
		if (inBattle) {
			if (!(fade_in || fade_out)) {
				if (music.getStatus() == sf::Music::Stopped)
					music.play();
				test.handleInput(event, ui_kb, elapsedTime);
			}
		}
		else if (endBattle && fade_in) {
			endBattle = false;
			music.openFromFile("resources/audio/" + musicName);
		}
		else {
			if (current_map != map_name && initial_load_map)
			{
				if (!fade_out && !fade_in) {
					fade_out = true;
					sysFader.resetFader();
				}
				else if (fade_in) {
					previousMusicName = musicName;
					load_map(ml, map_name, player, actors, textureMap, event_tiles, events, textbox, message, musicName, previous_maps, test);
					entities.clear();
					for (int i = actors.size(); i != 0; i--) {
						entities.push_back(actors[i - 1]);
					}
					previous_maps.push_back(map_name);
					current_map = map_name;
					if (previousMusicName != musicName) {
						music.openFromFile("resources/audio/" + musicName);
					}
				}
				// *************** End Audrey Edit *************** //
			}

			// if there is no UI visible, perform normal game actions
			if (music.getStatus() == sf::Music::Stopped)
				music.play();
			if (!UI_visible(sysWindows) && window.hasFocus())
			{
				worldAnimationArr[Map::Counter] += elapsedTime;


				if (!textbox) {
					// Audrey Edit: Adding Event class functionalities //
					if (eventIsRunning) {
						events[currentEvent].runEvent(elapsedTime);
						eventIsRunning = events[currentEvent].eventIsRunning();
						if ((events[currentEvent].getEventType()).substr(0, 6) == "Speech") {
							if (!textbox) {
								events[currentEvent].runEvent(elapsedTime);
								textbox = eventIsRunning;
							}
							else {
								textbox = eventIsRunning;
							}
						}
						else  if ((events[currentEvent].getEventType()).substr(0, 6) == "Battle") {
							if (test.getStartBattle()) {
								test.loadBattle(inBattle, sysFont, battleMenuTexture, cursorTexture, sfx_blip1);
								fade_out = true;
								sysFader.resetFader();
							}
						}
					}
					else {
						// *************** End Audrey Edit *************** //
						player.move(elapsedTime, player.controller.get_input());
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
		}
		// BEGIN DRAW CYCLE

		window.clear();
		window.setView(playerView);

		if (inBattle && !fade_out) {
			test.update(playerView.getCenter(), elapsedTime);
			window.draw(test);
			inBattle = test.getBattleState();
			if (!inBattle) {
				endBattle = true;
				fade_out = true;
			}
		}
		else if (!titlePtr->isVisible()) {

			animateMap(ml, window, worldAnimationArr);
			if (map_name == "title.tmx") {
				entities[1]->spriteAnimate(elapsedTime);
			}
			drawEntities(window, entities);
			ml.Draw(window, Layer::Overlay);

			if (textbox && !UI_visible(sysWindows)) {
				textbox = _Textbox->display_message(message, player, elapsedTime, ui_kb[sf::Keyboard::Return]);
			}

		}
		if (fade_out && !fade_in && !sysFader.isComplete()) {
			sysFader.setPosition(playerView.getCenter());
			sysFader.performFade(Fade::Out, Speed::Fast);
			window.draw(sysFader);
			std::cout << "Decreasing: Volume is " << music.getVolume() << std::endl;
			music.setVolume(music.getVolume() - 3);
		}
		else if (fade_out && !fade_in && sysFader.isComplete()) {
			fade_out = false;
			fade_in = true;
			if (start) {
				start = false;
				titlePtr->setVisible(false);
			}
			else if (inBattle) {
				music.openFromFile("resources/audio/LL_Battle.wav");
				music.play();
			}
			sysFader.resetFader();
			window.draw(sysFader.blackScreen());
		}
		else if (fade_in && !fade_out && !sysFader.isComplete()) {
			sysFader.setPosition(playerView.getCenter());
			sysFader.performFade(Fade::In, Speed::Fast);
			window.draw(sysFader);
			std::cout << "Increasing: Volume is " << music.getVolume() << std::endl;
			music.setVolume(music.getVolume() + 3);
		}
		else if (fade_in && !fade_out && sysFader.isComplete()) {
			fade_in = false;
			canPoll = true;
		}

		if (ui_kb.find(event.key.code) != ui_kb.end())
			ui_kb[event.key.code] = false;


		drawTextbox(window, _Textbox, textbox);
		drawUI(window, playerView, sysWindows, elapsedTime);




		window.display();
	}
	return 0;
}

/*********************************************************************
brief Performs the collision between actors.
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

std::string onEventTile(Character player, std::vector<EventTile> event_tiles, std::string lookingFor)
{
	sf::Vector2f playerpos = player.getPosition();
	for (int i = 0; i < event_tiles.size(); i++) {
		if (playerpos.x >= event_tiles[i].corner.x && playerpos.x <= event_tiles[i].corner.x + 64 && playerpos.y >= event_tiles[i].corner.y && playerpos.y <= event_tiles[i].corner.y + 64) {
			if (event_tiles[i].type == lookingFor) {
				return event_tiles[i].info;
			}
			else {
				return "";
			}
		}
	}
	return "";
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
void load_map(tmx::MapLoader& ml, std::string& map_name, Character& player, std::vector<Actor*>& actors, std::map<std::string, sf::Texture*> textureMap, std::vector<EventTile>& event_tiles, std::map<std::string, Event>& events, bool& textbox, string messages[], std::string &music_name, std::vector<std::string> previous_maps, BattleScene &battleScene) {
	int place = -1;
	for (int i = 0; i < previous_maps.size(); i++) { if (previous_maps[i] == map_name) { place = i; break; } }

	ml.Load(map_name);
	events.clear();
	for (int i = 1; i < actors.size(); i++) {
		delete actors[i];
	}
	if (actors.size() > 0) {
		actors.erase(actors.begin(), actors.end() - 1);
		actors[0] = &player;
	}
	else {
		actors.push_back(&player);
	}
	event_tiles.clear();
	std::map<std::string, std::vector<std::pair<Step*, Actor*>>> steps;

	for (auto layer = ml.GetLayers().begin(); layer != ml.GetLayers().end(); ++layer)
	{
		if (layer->name == "Events") {
			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++) {
				if (object->GetPropertyString("Event") == "trigger") {
					event_tiles.push_back({ object->GetPosition(), "trigger", object->GetPropertyString("EventType") });
					if (object->GetPropertyString("numTrigger") != std::string()) {
						for (int i = 0; i < std::stoi(object->GetPropertyString("numTrigger")); i++)
							steps[object->GetPropertyString("EventType")].push_back({ nullptr, nullptr });
					}
				}
				else {
					event_tiles.push_back({ object->GetPosition(), "map", object->GetPropertyString("Event") });
				}
			}
		}
		if (layer->name == "Setup")
		{
			if (layer->properties.find("script") != layer->properties.end()) {
				messages[0] = "resources/script/" + layer->properties["script"];
			}
			else {
				messages[0] = "resources/script/scenes.txt";
			}
			music_name = layer->properties["music"];
			if (place == -1) {
				if (layer->properties["hasPriority"] == "Yes") { events["Story"].setPriority(true); }
				else { events["Story"].setPriority(false); }
				if (layer->properties.find("numEvents") != layer->properties.end()) {
					for (int i = 0; i < std::stoi(layer->properties["numEvents"]); i++) {
						steps["Story"].push_back({ nullptr, nullptr });
					}
				}
			}
			for (auto object = layer->objects.begin(); object != layer->objects.end(); object++)
			{
				if (object->GetName() == "START") {
					player.setPosition(object->GetCentre());
					if (object->GetPropertyString("Texture") == "Hide") {
						player.changeSprite(*textureMap["Empty"]);
					}
					else {
						player.changeSprite(*textureMap["Warren"]);
					}
					if (object->GetPropertyString("Direction") != std::string()) {
						player.setDirection(_directionOfActor(object->GetPropertyString("Direction")));
					}
				}
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
				// Adding
				if (place == -1) {
					std::string order[2] = { object->GetPropertyString("Event Order"), object->GetPropertyString("TriggerEventOrder") };
					for (int j = 0; j < 2; j++) {
						if (order[j] != "") {
							std::string eventType = (j == 0) ? "Story" : "A";
							for (int i = 0; i < order[j].size(); i++)
							{
								if (order[j][i] == ';') {
									continue;
								}
								std::string key = std::string(1,order[j][i]);
								std::string e;
								if (j == 1) {
									if (steps.find(key) != steps.end()) {
										eventType = key;
										e = object->GetPropertyString(key + "Trigger" + order[j][++i]);
									}
									else {
										key = eventType;
										e = object->GetPropertyString(key + "Trigger" + order[j][i]);
									}
									if ((unsigned int)order[j][i] >= 97) {
										std::cout << "Key is " << ((int)order[j][i]) << " or " << (order[j][i]) << std::endl;
										order[j][i] -= '\'';
									}
								}
								else {
									e = object->GetPropertyString(key);
								}

								Actor* moving_actor = (object->GetName() == "START") ? actors[0] : actors[actors.size() - 1];
								Step* step = nullptr;
								switch (e[0] - '0') {
								case 0: // Move Step
									step = new MoveStep(std::vector<sf::Vector2f>({ tile(std::stoi(e.substr(1, 2)), std::stoi(e.substr(4, 2))) }));
									break;
								case 1: // Together Step
									if (steps[eventType][order[j][i] - '0' - 1].first == nullptr) {
										step = new TogetherStep(std::vector<sf::Vector2f>({ tile(std::stoi(e.substr(1, 2)), std::stoi(e.substr(4, 2))) }), std::vector<Actor*>({ moving_actor }));
									}
									else {
										steps[eventType][order[j][i] - '0' - 1].first->addToStep(sf::Vector2f({ tile(std::stoi(e.substr(1, 2)), std::stoi(e.substr(4, 2))) }), moving_actor);
									}
									break;
								case 2: // Direction Step
									step = new DirectionStep(Direction(e[1] - '0'));
									break;
								case 3: // Speech Step
									step = new SpeechStep(messages, textbox);
									moving_actor->addEventDialogue(e.substr(1));
									break;
								case 4:
									step = new MapStep(map_name, e.substr(1));
									break;
								case 5:
									step = new BattleStep(&battleScene, e.substr(1));
								}
								if (step != nullptr) {
									int index = order[j][i] - '0' - 1;
									steps[eventType][order[j][i] - '0' - 1] = (std::pair<Step*, Actor*>(step, moving_actor));
								}
							}

						}
					}
				}
			}
		}
	}
	for (auto step : steps) {
		for (auto e : step.second) {
			events[step.first].addEvents(e.first, e.second);
		}
	}
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