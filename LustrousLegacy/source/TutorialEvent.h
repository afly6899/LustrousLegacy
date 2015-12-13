#ifndef __RPG_Town__TutorialEvent__
#define __RPG_Town__TutorialEvent__

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <queue>
#include <initializer_list>
#include "player.h"
#include "Enums.h"


struct TutorialEvent{
    Direction direction;
    int stop_distance;
};

class Event { // this is purely for movement!
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

    
    
//    class TutorialEvent {
//    public:
//        ~TutorialEvent();
//        TutorialEvent(Direction, int);
//        
//        void move(Player& player, float elapsedTime);
//        //    void setSpeed(int new_speed);
//        //    void setDirection(Direction direction);
//        //    void setStopPosition(int stop_after);
//        
//        bool eventIsDone(Player& player, int start);
//        
//        
//    private:
//        TutorialEvent* next;
//        Direction dir;
//        int stop_distance;
//        int speed = 2;
//        bool collision = false;
//        bool auto_move = true;
//    };
};

#endif /* defined(__RPG_Town__TutorialEvent__) */
