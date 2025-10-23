#pragma once
#include <SFML/Graphics.hpp>
class Entity
{
  public:
	Entity();

	~Entity();
	virtual void draw(sf::RenderWindow& window);
	void init();
	virtual void updateAnimation(float dt);
	virtual void updateSpriteRect();
	

  protected:
	  
	  sf::Texture m_texture;
	  sf::Sprite m_sprite{ m_texture };

	  float m_animationTimer = 0.f;
	  float m_animationDelay = 0.15f;

	  int m_currentFrame = 0;
	  int m_firstFrame = 0;
	  int m_lastFrame = 3;
	  int frameWidth = 256;
	  int frameHeight = 384;

};

