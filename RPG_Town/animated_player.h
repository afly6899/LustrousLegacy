#ifndef ANIMATED_PLAYER_
#define ANIMATED_PLAYER_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

// Forward Declaration
namespace sf {
    class Texture;
}

namespace actor {
    
    class AnimatedPlayer : public sf::Drawable {
    public:
        enum Direction {
            South, West, East, North
        };
        
        enum States {
            Idle, Walking
        };
        
        // default constructor - requires a player texture
        AnimatedPlayer(const sf::Texture& playerTexture);
        
        // default destructor
        ~AnimatedPlayer();
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateIdle(bool& animat);
        void idle();
        
        
    private:
        
        sf::Sprite mSprite;
        sf::Vector2i mSource;
        int idleCounter = 0; // counter for when player is idle
        int walkCounter = 0; // for when player is walking
        int aniFrameDuration = 10; // number of times counter needs to increment to before the frame changes
    };
}

#endif /* defined(Animated_Player_) */
