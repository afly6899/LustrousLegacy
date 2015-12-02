#include <cmath>
#include <queue>
#include <iostream>
#include "Enums.h"
#include "Event.h"
#include "player.h"
#include "NPC.h"

// Defining methods for Event

Event::~Event() {}

Event::Event() {}

Event::Event(const std::initializer_list<TutorialEvent> events) {
    event_chain = std::queue<TutorialEvent>(events);
}


// Mutators/Accessors
void Event::addEvent(Direction dir, int stop_at) {
    TutorialEvent to_add = {dir, stop_at};
    event_chain.push(to_add);
}


void Event::addEvent(TutorialEvent to_add) {
    event_chain.push(to_add);
}


bool Event::fullEventIsDone() {
    return event_chain.empty();
}



bool Event::currentEventIsDone(Player& player) {
    if (current_event.stop_distance == 0) {
        return player.getDirection() == current_event.direction;
    }
    
    int distance_traveled;
    if (current_event.direction == Direction::West || current_event.direction == Direction::East)
        distance_traveled = std::abs(player.getPosition().x - start_position.x);
    else
        distance_traveled = std::abs(player.getPosition().y - start_position.y);
    
    return distance_traveled == current_event.stop_distance*64;
}


void Event::doEvent(Player& player, float elapsedTime) {
    if (current_event.stop_distance == 0) {
        player.faceDirection(current_event.direction);
    } else {
    player.move(speed, elapsedTime, collision, auto_move, current_event.direction);
    }
}


void Event::runEvent(bool& event_happening, Player& player, float elapsedTime) {
    if (event_happening) { // make sure nothing happens when there's no events left
        if (current_event.direction == Direction::Null) {
            start_position = player.getPosition();
            nextEvent();
        } else if (currentEventIsDone(player)) {
            if (fullEventIsDone()) { // reset everything
                event_chain = event_keeper;
                event_keeper = std::queue<TutorialEvent>();
                current_event = {Direction::Null, -1};
                event_happening = false;
            } else {
                start_position = player.getPosition();
                nextEvent();
            }
        }
        if (event_happening) {
            doEvent(player, elapsedTime);
        }
    }
}


void Event::nextEvent() {
    current_event = event_chain.front();
    event_keeper.push(event_chain.front());
    event_chain.pop();
}


//void Event::goToNPC(NPC npc) {
//    
//}
