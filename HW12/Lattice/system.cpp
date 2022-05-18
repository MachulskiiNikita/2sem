#include "system.hpp"

void System::initialize()
{
	const auto size = std::size(m_particles);

	for (auto i = 0U; i < size-1; ++i)
	{
		for (auto j = i+1; j < size; ++j)
		{
			m_links.push_back(Link(particle(i), particle(j), m_stiffness));
		}
	}
}

void System::push(const sf::Vector2f force) const
{
	for (auto i = 0U; i < std::size(m_particles); ++i)
	{
		m_particles[i]->move(force);
	}
}

void System::update() const
{
	
	
	for (auto i = 0U; i < std::size(m_particles); ++i)
	{

		auto acceleration = m_particles[i]->acceleration();
		if (m_particles[i]->position().y + m_particles[i]->radius() > m_max_point.y)
		{
			//m_particles[i]->set_y(m_max_point.y - m_particles[i]->radius());
			m_particles[i]->set_velocity_y(0.0f);
			if (acceleration.y > 0) {
				m_particles[i]->move((acceleration.x, 0.0f));
			}
		}

		if (m_particles[i]->position().y - m_particles[i]->radius() < m_min_point.y)
		{
			//m_particles[i]->set_y(m_min_point.y + m_particles[i]->radius());
			m_particles[i]->set_velocity_y(0.0f);
			if (acceleration.y < 0) {
				m_particles[i]->move((acceleration.x, 0.0f));
			}
		}

		if (m_particles[i]->position().x + m_particles[i]->radius() > m_max_point.x)
		{
			//m_particles[i]->set_x(m_max_point.x - m_particles[i]->radius());
			m_particles[i]->set_velocity_x(0.0f);
			if (acceleration.x > 0) {
				m_particles[i]->move((0.0f, acceleration.y));
			}
		}

		if (m_particles[i]->position().x - m_particles[i]->radius() < m_min_point.x)
		{
			//m_particles[i]->set_x(m_min_point.x + m_particles[i]->radius());
			m_particles[i]->set_velocity_x(0.0f);
			if (acceleration.x < 0) {
				m_particles[i]->move((0.0f, acceleration.y));
			}
		}
		for (auto j = 0U; j < std::size(m_links); ++j)
		{
			m_links[j].update();

		}
		m_particles[i]->move(0.25f);
		
	}
	
	

}