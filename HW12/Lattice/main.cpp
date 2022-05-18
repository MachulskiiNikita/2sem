#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "system.hpp"
#include <iostream>

#include <chrono>
#include <thread>




int main(int argc, char ** argv)
{
	sf::RenderWindow window(sf::VideoMode(800U, 600U), "PHYSICS");

	sf::Vector2f min_point(  10.0f,   10.0f);
	sf::Vector2f max_point(765.0f, 565.0f);

	const auto C = (min_point + max_point) * 0.2f;

	const auto r = 2.0f;

	const auto R = length(max_point - min_point) * 0.08f;

	const auto A = 7U;

	const auto L = 20.0f;

	const auto radius_interaction = 60.0f;

	const auto stiffness = 0.001f;


	std::vector < System::particle_t > particles;

	for (auto i = 0U; i < A; i++)
	{
		for (auto j = 0U; j < A; j++)
		{

			auto position = sf::Vector2f(i * L, j * L) + C;

			particles.push_back(std::make_shared < Particle >(position,  sf::Vector2f(0.0f, 8.0f), r, i % 2));
		}
	}

	/*for (auto i = 0U; i < A +1; i++)
	{
		for (auto j = 0U; j < A - i%2; j++)
		{

			auto position = sf::Vector2f(i * L , 2* j * L + L * (i % 2)) + C;

			particles.push_back(std::make_shared < Particle >(position,  sf::Vector2f(0.0f, 8.0f), r, i%2));
		
		}
	}*/


	System system(min_point, max_point, particles, A, stiffness);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			system.push(sf::Vector2f(0.0f, -0.2f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			system.push(sf::Vector2f(0.0f, 0.2f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			system.push(sf::Vector2f(-0.2f, 0.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			system.push(sf::Vector2f(0.2f, 0.0f));
		}
		system.update();
		
		window.clear();
		
		for (auto i = 0U; i < system.particles().size(); ++i)
		{
			sf::CircleShape circle(2.0f * r);

			circle.setPosition(system.particle(i)->position() + sf::Vector2f(r, r));
			if (system.particle(i)->is_negative()) {
				circle.setFillColor(sf::Color::Red);
			}
			else {
				circle.setFillColor(sf::Color::Green);
			}

			window.draw(circle);
		}
		window.display();
	}
	
	// system("pause");

	return EXIT_SUCCESS;
}
