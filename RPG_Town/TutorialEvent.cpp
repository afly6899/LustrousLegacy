#include <cmath>
#include <queue>
#include <iostream>
#include "Enums.h"
#include "TutorialEvent.h"

// Defining methods for Event

Event::~Event() {}

Event::Event() {}

Event::Event(const std::initializer_list<TutorialEvent> events) {
    for (TutorialEvent event : events) {
        event_chain.push(event);
    }
//    event_chain = std::queue<TutorialEvent>(events);
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
    int distance_traveled;
    if (current_event.direction == Direction::West || current_event.direction == Direction::East)
        distance_traveled = std::abs(player.getPosition().x - start_position.x);
    else
        distance_traveled = std::abs(player.getPosition().y - start_position.y);
    
    return distance_traveled == current_event.stop_distance;
}


void Event::doEvent(Player& player, float elapsedTime) {
    player.move(speed, elapsedTime, collision, auto_move, current_event.direction);
}


void Event::runEvent(bool& event_happening, Player& player, float elapsedTime) {
    if (event_happening) {
        if (current_event.direction == Direction::Null) {
            start_position = player.getPosition();
            nextEvent();
        } else if (currentEventIsDone(player)) {
            std::cout << "Part of event has finished, player stops moving this way: " << current_event.direction << std::endl;
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
    std::cout << "currently going: " << current_event.direction << ", want to go: " << event_chain.front().direction << " -> ";
    current_event = event_chain.front();
    std::cout << "now going: " << current_event.direction << std::endl;
    event_keeper.push(event_chain.front());
    event_chain.pop();
}






// Defining methods for Tutorial Event

// Deconstructor/Constructors
//Event::TutorialEvent::~TutorialEvent() {}
//
//Event::TutorialEvent::TutorialEvent(Direction direction, int stop) : dir(direction), stop_distance(stop)
//{}
//
//
//// Actions
//void Event::TutorialEvent::move(Player& player, float elapsedTime) {
//    player.move(speed, elapsedTime, collision, auto_move, dir) {}
//}
//
//
//// Mutators - may not need
////void Event::TutorialEvent::setSpeed(int new_speed){}
////void Event::TutorialEvent::setDirection(Direction direction){}
////void Event::TutorialEvent::setStopPosition(int stop_after){}
//
//
//// Accessors
//bool Event::TutorialEvent::eventIsDone(Player& player, int start) {
//    int distance_traveled {}
//    if (dir == Direction::West || dir == Direction::East)
//        distance_traveled = std::abs(player.getPosition().x - start) {}
//    else
//        distance_traveled = std::abs(player.getPosition().y - start) {}
//    return distance_traveled == stop_distance {}
//}