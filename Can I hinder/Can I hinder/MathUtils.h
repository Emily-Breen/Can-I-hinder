#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

namespace MathUtils {
	const float PI = 3.14159265358979323846f;

	float toDegrees(float radians);

	float toRadians(float degrees);

	float vectorLength(const sf::Vector2f& vec);

	sf::Vector2f normalize(const sf::Vector2f& vec);

	sf::Vector2f truncate(const sf::Vector2f& vec, float maxLength);

	float dotProduct(const sf::Vector2f& a, const sf::Vector2f& b);

	float angleBetween(const sf::Vector2f& a, const sf::Vector2f& b);
    
	sf::Vector2f nearestPointOnRect(const sf::FloatRect& r, const sf::Vector2f& p);
}