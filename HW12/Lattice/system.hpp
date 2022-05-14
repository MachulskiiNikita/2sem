#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "link.hpp"
#include "particle.hpp"

class System
{
public:

	using particle_t = std::shared_ptr < Particle > ;

public:

	explicit System(sf::Vector2f min_point, sf::Vector2f max_point, std::vector < particle_t > & particles, const unsigned int A, const float radius_interaction, const float stiffness) noexcept :
			m_min_point(min_point), m_max_point(max_point), 
			m_particles(particles), m_A(A), m_radius_interaction(radius_interaction), m_stiffness(stiffness)
	{
		initialize();
	}

	~System() noexcept = default;

private:

	void initialize();

public:

	particle_t particle(std::size_t index) const
	{
		return m_particles.at(index);
	}

	const auto & particles() const noexcept
	{
		return m_particles;
	}

public:
	
	void push(sf::Vector2f force) const;

	void update() const;

private:

	const unsigned int m_A;
	const float m_radius_interaction;
	const float m_stiffness;
	sf::Vector2f m_min_point;
	sf::Vector2f m_max_point;

	std::vector < particle_t > m_particles;

	std::vector < Link > m_links;
};