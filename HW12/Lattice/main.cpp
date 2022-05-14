#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "system.hpp"
#include <iostream>

int main(int argc, char ** argv)
{
	sf::RenderWindow window(sf::VideoMode(800U, 600U), "PHYSICS");

	sf::Vector2f min_point(  0.0f,   0.0f);
	sf::Vector2f max_point(775.0f, 575.0f);

	const auto C = (min_point + max_point) * 0.2f;

	const auto r = 2.0f;

	const auto R = length(max_point - min_point) * 0.08f;

	const auto A = 10U;

	const auto L = 20.0f;

	const auto radius_interaction = 60.0f;

	const auto stiffness = 10.0f;


	std::vector < System::particle_t > particles;

	for (auto i = 0U; i < A; i++)
	{
		for (auto j = 0U; j < A; j++)
		{

			auto position = sf::Vector2f(i * L, j * L) + C;

			particles.push_back(std::make_shared < Particle >(position, position, sf::Vector2f(0.0f, 8.0f), r, (i+j)%2));
			std::cout << i << " " << j << " " << (i + j) % 2 << '\n';
		}
	}


	System system(min_point, max_point, particles, A, radius_interaction, stiffness);

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

		system.update();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			system.push(sf::Vector2f(0.0f, -2.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			system.push(sf::Vector2f(0.0f, 2.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			system.push(sf::Vector2f(-2.0f, 0.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			system.push(sf::Vector2f(2.0f, 0.0f));
		}

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
