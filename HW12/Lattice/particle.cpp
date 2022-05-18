#include "particle.hpp"

void Particle::move(sf::Vector2f force)
{
	m_acceleration = force  + m_gravity * 0.00001f;
}

void Particle::move(float dt)
{

	m_velocity += m_acceleration * dt ;
	m_velocity = m_velocity * 0.99f;

	m_position += m_velocity * dt ;
	m_acceleration = 0.0f * m_acceleration;

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
//	m_position += ((m_position - m_previous_position) * dt);// +m_gravity * dt * dt);
//
//	m_previous_position = current_position;
//}