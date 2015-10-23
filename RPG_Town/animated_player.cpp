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
        endFramePlace = (int)mSprite.getTexture()->getSize().x; // initialize when you have texture
    }
    
    
    AnimatedPlayer::~AnimatedPlayer() {
    }
    
    
    void AnimatedPlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mSprite, states);
    }
    
    void AnimatedPlayer::updateIdle() {
        if (idleCounter == aniFrameDuration - 1) {
            mSource.x = (mSource.x + 1) % endFramePlace;
            mSprite.setTextureRect(sf::IntRect(mSource.x * 23, mSource.y * 22, 23, 22));
        }
        idleCounter = (idleCounter + 1) % aniFrameDuration;
    }
    
    
    
    void AnimatedPlayer::idle() {
        mSource.x = 1;
        mSprite.setTextureRect(sf::IntRect(mSource.x * 23, mSource.y * 22, 23, 22));
    }
}