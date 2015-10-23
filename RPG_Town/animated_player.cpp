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
        mSprite.setTextureRect(sf::IntRect(mSource.x * 32, mSource.y * 32, 32, 32));
        endFramePlace = (int)mSprite.getTexture()->getSize().x/32; // initialize when you have texture - want to get the size and divide it by the size of a sprite image!
    }
    
    
    AnimatedPlayer::~AnimatedPlayer() {
    }
    
    
    void AnimatedPlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mSprite, states);
    }
    
    void AnimatedPlayer::updateIdle() {
        if (idleCounter == aniFrameDuration - 1) {
            mSource.x = (mSource.x + 1) % endFramePlace;
            mSprite.setTextureRect(sf::IntRect(mSource.x * 32, mSource.y * 32, 32, 32));
        }
        idleCounter = (idleCounter + 1) % aniFrameDuration;
    }
    
    
    
    void AnimatedPlayer::idle() {
        mSource.x = 1;
        mSprite.setTextureRect(sf::IntRect(mSource.x * 32, mSource.y * 32, 32, 32));
    }
}