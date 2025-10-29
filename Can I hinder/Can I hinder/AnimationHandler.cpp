#include "AnimationHandler.h"
/// <summary>
/// ANIMATION
/// </summary>
/// <param name="texture"></param>
/// <param name="startFrame"></param>
/// <param name="frameCount"></param>
/// <param name="frameDuration"></param>
/// <param name="offsetY"></param>
Animation::Animation(sf::Texture& texture, int startFrame, int frameCount,
	float frameDuration, int offsetX, int offsetY,
	int frameWidth)
	: frameTime(frameDuration)
{
	const int frameHeight = 48;

	for (int i = 0; i < frameCount; ++i)
	{
		m_frames.emplace_back(
			sf::Vector2i(offsetX + i * frameWidth, offsetY),
			sf::Vector2i(frameWidth, frameHeight)
		);
	}
}

void Animation::update(float dt)
{
	currentTime += dt;
	if (currentTime >= frameTime) {
		currentTime = 0.f;
		currentFrame = (currentFrame + 1) % m_frames.size();
	}
}

void Animation::applyToSprite(sf::Sprite& sprite) const
{
	sprite.setTextureRect(m_frames[currentFrame]);
}

void Animation::reset()
{
	currentFrame = 0;
	currentTime = 0.f;
}

/// <summary>
/// ANIMATION HANDLER
/// </summary>
/// <param name="t_texture"></param>
AnimationHandler::AnimationHandler(sf::Texture& t_texture) : m_texture(t_texture)
{

}

void AnimationHandler::addAnimation(PlayerState m_state, Direction m_direction, int startFrame, int frameCount, float frameDuration, int offsetX, int offsetY, int frameWidth)
{
	m_animations[{m_state, m_direction}] =
		Animation(m_texture, startFrame, frameCount, frameDuration,
			offsetX, offsetY, frameWidth);
}

void AnimationHandler::changeState(PlayerState newState)
{
	if (m_currentState != newState) {
		m_currentState = newState;
		m_animations[{m_currentState, m_currentDirection}].reset();
	}
}

void AnimationHandler::changeDirection(Direction newDirection)
{
	if (m_currentDirection != newDirection) {
		m_currentDirection = newDirection;
		m_animations[{m_currentState, m_currentDirection}].reset();
	}
}

void AnimationHandler::update(float dt)
{
	m_animations[{m_currentState, m_currentDirection}].update(dt);
}

void AnimationHandler::applyToSprite(sf::Sprite& sprite)
{
	m_animations[{m_currentState, m_currentDirection}].applyToSprite(sprite);
}
