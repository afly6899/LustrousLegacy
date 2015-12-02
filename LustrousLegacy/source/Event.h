#ifndef __RPG_Town__Event__
#define __RPG_Town__Event__

#include <queue>
#include <initializer_list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"
#include "Enums.h"
#include "NPC.h"


struct TutorialEvent{
    Direction direction;
    int stop_distance;
};

class Event { // this is purely for movement!
public:
    
    // Constructors and Destructors
    ~Event();
    Event(); // make an empty Event that can have stuff added to it (necessary?)
    Event(const std::initializer_list<TutorialEvent> events); // initialize a group of TutorialEvents
    
    // Adding Events
    void addEvent(Direction dir, int stop_at); // creates a TutorialEvent and adds it to queue
    void addEvent(TutorialEvent); // override to account for getting either thing
    
    // Accessors - may be used by outside
    bool fullEventIsDone(); // returns true if there's nothing in the queue (necessary?)
    bool currentEventIsDone(Player& player); // returns true if current event is done
    
    // FOR DOING THE ACTUAL EVENT
    void doEvent(Player& player, float elapsedTime); // carries out move/face function
    void runEvent(bool& event_happening, Player& player, float elapsedTime); // takes care of running each event
    void nextEvent(); // changes the current_event to the next one once the current one is done
    
    // Processing NPC Location
//    void goToNPC(NPC npc);
    
private:
    std::queue<TutorialEvent> event_chain; // keeps all the events
    std::queue<TutorialEvent> event_keeper; // this is for repeatable events
    TutorialEvent current_event = {Direction::Null, -1}; // tells what movement the player will do
    int speed = 8; // for Player::move
    sf::Vector2f start_position; // keep track of where the player started for distance calculations
    bool collision = false; // for Player::move
    bool auto_move = true; // for Player::move

};

#endif /* defined(__RPG_Town__Event__) */
