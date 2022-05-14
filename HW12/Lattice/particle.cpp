#include "particle.hpp"

void Particle::move(sf::Vector2f force)
{
	m_acceleration = force*0.1f ;
}

void Particle::move(float dt)
{

	m_velocity += m_acceleration * dt ;

	m_position += m_velocity * dt ;

}
//#include "particle.hpp"
//
//void Particle::move(sf::Vector2f delta)
//{
//	m_position += delta;
//}
//
//void Particle::move(float dt)
//{
//	const auto current_position = m_position;
//
//	m_position += ((m_position - m_previous_position) * dt + m_acceleration * dt * dt);
//
//	m_previous_position = current_position;
//}