#pragma once
#include <SFML/Graphics.hpp>
class Entity
{
  public:
	Entity();

	~Entity();
	virtual void draw(sf::RenderWindow& window);
	virtual void update(float dt) = 0;
	sf::Sprite getSprite();

  protected:
	  
	  sf::Texture m_texture;
	  sf::Sprite m_sprite{ m_texture };

	  

};

