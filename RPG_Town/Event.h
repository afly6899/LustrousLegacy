#ifndef __RPG_Town__Event__
#define __RPG_Town__Event__

#include <queue>
#include <initializer_list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "player.h"
#include "Enums.h"

// Defines where and for how many pixels the player moves
struct TutorialEvent{
    Direction direction;
    int stop_distance;
};

// Class that handles a bunch of TutorialEvent objects and has them run
class Event {
public:
    ~Event();
    Event();
    Event(int number_of_events);
    Event(const std::initializer_list<TutorialEvent> events);
    
    void addEvent(Direction dir, int stop_at); // creates a TutorialEvent and adds it to queue
    void addEvent(TutorialEvent); // override to account for getting either thing
    bool fullEventIsDone(); // returns true if there's nothing in the queue
    
    bool currentEventIsDone(Player& player); // returns true if current event is done
    void doEvent(Player& player, float elapsedTime);
    void runEvent(bool& event_happening, Player& player, float elapsedTime);
    void nextEvent();
    
private:
//    TutorialEvent* event_chain = nullptr;
    std::queue<TutorialEvent> event_chain;
    std::queue<TutorialEvent> event_keeper; // this is too keep the events, change later
    TutorialEvent current_event = {Direction::Null, -1};
    int speed = 4;
    sf::Vector2f start_position;
    bool collision = false;
    bool auto_move = true;
};

#endif /* defined(__RPG_Town__Event__) */
