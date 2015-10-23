#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "animated_player.h"

namespace actor {
    
    AnimatedPlayer::AnimatedPlayer(const sf::Texture& playerTexture) :
    mSprite(playerTexture),
    mSource(1, AnimatedPlayer::South) {
        mSprite.setScale(2.0f, 2.0f);
        mSprite.setTextureRect(sf::IntRect(mSource.x * 23, mSource.y * 22, 23, 22));
    }
    
    
    AnimatedPlayer::~AnimatedPlayer() {
    }
    
    
    void AnimatedPlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mSprite, states);
    }
    
    void AnimatedPlayer::update(bool& animated) {
        mSource.x++;
        animated = true;
        if (mSource.x*23 >= (int)mSprite.getTexture()->getSize().x) {
            mSource.x = 0;
        }
        else if (mSource.x == 1 && animated) {
            animated = false;
        }
        mSprite.setTextureRect(sf::IntRect(mSource.x * 23, mSource.y * 22, 23, 22));
    }
    
    
    
    void AnimatedPlayer::idle() {
        mSource.x = 1;
        mSprite.setTextureRect(sf::IntRect(mSource.x * 23, mSource.y * 22, 23, 22));
    }
}