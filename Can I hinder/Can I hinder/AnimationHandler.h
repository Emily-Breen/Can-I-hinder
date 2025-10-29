#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
enum class PlayerState {
    IDLE,
    WALK,
    ATTACK,
    HURT,
    DEATH
};
enum class Direction {
    DOWN,
    LEFT,
    RIGHT,
    UP
};

class Animation
{
  public:
    Animation() = default;
    Animation(sf::Texture& texture, int startFrame, int frameCount, float frameDuration, int offsetX = 0, int offsetY = 0, int frameWidth =0);
	void update(float dt);
	void applyToSprite(sf::Sprite& sprite) const;
    void reset();

  private:
      std::vector<sf::IntRect> m_frames;
	  float frameTime{ 0.f };
      float currentTime{ 0.f };
      int currentFrame{ 0 };
};
class AnimationHandler 
{
   public:
       AnimationHandler(sf::Texture& t_texture);
	   void addAnimation(PlayerState t_state, Direction t_direction, int startFrame, int frameCount, float frameDuration, int offsetX = 0, int offsetY = 0, int frameWidth = 0);
       void changeState(PlayerState newState);
	   void changeDirection(Direction newDirection);
       void update(float dt);
	   void applyToSprite(sf::Sprite& sprite);
   private:
	   sf::Texture& m_texture;
	   std::map<std::pair<PlayerState, Direction>, Animation> m_animations;
	   PlayerState m_currentState{ PlayerState::IDLE };
	   Direction m_currentDirection{ Direction::DOWN };

};
