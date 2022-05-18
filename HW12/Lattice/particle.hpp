#pragma once

#include <SFML/Graphics.hpp>

class Particle
{
public:

	Particle() noexcept = default;

	explicit Particle(sf::Vector2f position, 
		sf::Vector2f gravity, float radius, bool is_negative) noexcept :
			m_position(position),
			m_gravity(gravity), m_radius(radius), m_is_negative(is_negative), m_acceleration(0.0f,0.0f)
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

	const auto acceleration() const noexcept
	{
		return m_acceleration;
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

	void set_velocity_x(float v) noexcept
	{
		m_velocity.x = v;
	}
	
	void set_velocity_y(float v) noexcept
	{
		m_velocity.y = v;
	}

public:

	void move(sf::Vector2f delta);

	void move(float dt);

private:

	sf::Vector2f m_position;
	sf::Vector2f m_gravity;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;

	int m_is_negative ;

	float m_radius;
};