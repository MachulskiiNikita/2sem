//#include "link.hpp"
//#include <math.h>
//
//void Link::update() const
//{
//	const auto delta = m_particle_2->position() - m_particle_1->position();
//	if (m_particle_2->is_negative() == m_particle_1->is_negative()) {
//		m_particle_2->move(m_particle_2 ->acceleration() + norm(delta) / length(delta) / length(delta)  * m_stiffness*1.1f);
//		m_particle_1->move(m_particle_1 ->acceleration() + (norm(delta) / length(delta) / length(delta)  * m_stiffness) * -1.1f);
//	}
//	else {
//		m_particle_1->move(m_particle_1 ->acceleration() + norm(delta) / length(delta) / length(delta)  * m_stiffness );
//		m_particle_2->move(m_particle_2 ->acceleration() + (norm(delta) / length(delta) / length(delta)  * m_stiffness) * -1.0f);
//	}
//}
#include "link.hpp"

void Link::update() const
{
	const auto delta = m_particle_2->position() - m_particle_1->position();

	m_particle_1->move(m_particle_1 ->acceleration() +norm(delta) * (length(delta) - m_distance) * m_stiffness);
	m_particle_2->move(m_particle_2->acceleration() + norm(delta) * (length(delta) - m_distance) * m_stiffness * -1.0f);
}