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
        virtual void update(bool& animated);
        void idle();
        
        
    private:
        
        sf::Sprite mSprite;
        sf::Vector2i mSource;
        int aniCounter = 0;
        int aniFrameDuration = 200;
    };
}

#endif /* defined(Animated_Player_) */
