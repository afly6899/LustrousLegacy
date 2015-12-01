//
//  TextButton.h
//  RPG_Town
//
//  Created by Audrey Lai on 11/23/15.
//  Copyright (c) 2015 Audrey Lai. All rights reserved.
//

#ifndef __RPG_Town__TextButton__
#define __RPG_Town__TextButton__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class TextButton : public sf::Drawable {
public:
    bool inButtonRegion(int pos_x, int pos_y);
    
    
private:
    sf::Text text;
    bool clickable = false;
    
    
};

#endif /* defined(__RPG_Town__TextButton__) */
