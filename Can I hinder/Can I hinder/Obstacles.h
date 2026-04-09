#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Obstacles
{
public:
    Obstacles(std::shared_ptr<sf::Texture> tex, sf::Vector2f pos):m_texture(std::move(tex)),
        m_sprite(*m_texture)
    {
        m_sprite.setPosition(pos);
        m_sprite.setOrigin({ 32.f, 32.f });
        m_sprite.setScale({ 2.f, 2.f });
        m_lifetime = 5.0f;
    }

    void update(float) {}

    void draw(sf::RenderWindow& window)
    {
        window.draw(m_sprite);
    }

    sf::FloatRect getBounds() const
    {
        return m_sprite.getGlobalBounds();
    }
    bool isExpired() const
    {
        return m_lifetimeClock.getElapsedTime().asSeconds() >= m_lifetime;
    }

private:
    std::shared_ptr<sf::Texture> m_texture;
    sf::Sprite m_sprite;
	sf::Clock m_lifetimeClock;
    float m_lifetime;
};