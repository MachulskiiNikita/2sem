#pragma once

#include <SFML/Graphics.hpp>

class Particle
{
public:

	Particle() noexcept = default;

	explicit Particle(sf::Vector2f position, sf::Vector2f previous_position,
		sf::Vector2f acceleration, float radius, bool is_negative) noexcept :
			m_position(position), m_previous_position(previous_position),
			m_acceleration(acceleration), m_radius(radius), m_is_negative(is_negative)
	{}

	~Particle() noexcept = default;

public:

	const auto position() const noexcept 
	{ 
		return m_position; 
	}

	const auto radius() const noexcept
	{
		return m_radius;
	}

	const auto is_negative() const noexcept
	{
		return m_is_negative;
	}

public:

	void set_x(float x) noexcept
	{
		m_position.x = x;
	}

	void set_y(float y) noexcept
	{
		m_position.y = y;
	}

public:

	void move(sf::Vector2f delta);

	void move(float dt);

private:

	sf::Vector2f m_position;
	sf::Vector2f m_previous_position;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_velocity;
	bool m_is_negative ;

	float m_radius;
};