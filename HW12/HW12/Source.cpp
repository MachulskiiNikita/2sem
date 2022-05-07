#include <algorithm>
#include <array>
#include <thread>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

std::mt19937_64 mt;
std::uniform_int_distribution<> uid(0,3);
std::bernoulli_distribution bd(0.1);
enum class Direction
{
	down,
	left,
	right,
	up
};

class Particle
{
public:
	Particle(int coord_1, int coord_2, Direction dir) : m_x(coord_1), m_y(coord_2), direction(dir) {}
	void step()
	{
		switch (direction)
		{
		case Direction::left:
			if (m_x == 0) { m_x = 999; }
			else { --m_x; }
			break;
		case Direction::right:
			if (m_x == 999) { m_x = 1; }
			else { ++m_x; }
			break;
		case Direction::up:
			if (m_y == 0) { m_y = 999; }
			else { --m_y; }
			break;
		case Direction::down:
			if (m_y == 999) { m_y = 1; }
			else { ++m_y; }
			break;
		default:
			break;
		}
		if (bd(mt))
		{
			direction = static_cast<Direction>(uid(mt));
		}
	}
public:
	int x() { return m_x; }
	int y() { return m_y; }

private:
	int m_x;
	int m_y;
	Direction direction;

};

class Visualizer
{
public:
	Visualizer() :
		m_window(sf::VideoMode(m_width, m_height), m_window_name),
		m_field(m_squares_in_column, std::vector<std::size_t>(m_squares_in_column, 0))
	{
		std::uniform_int_distribution uid(0, static_cast<int>(m_width) - 1);
		particles.reserve(m_particles_num);
		for (auto i = 0; i < m_particles_num; ++i)
		{
			auto x = uid(mt);
			auto y = uid(mt);
			auto dir = static_cast<Direction>(uid(mt));
			++m_field[x / squares_size][y / squares_size];
			particles.emplace(std::end(particles), Particle(x, y, dir));
		}
		while (m_window.isOpen())
		{
			sf::Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					m_window.close();
				}
			}
			move();
			m_window.clear();
			draw();

		
	};

	}

private:


	void move()
	{
		std::for_each(std::begin(particles), std::end(particles), [this](auto& particle) {particle.step(); });
	}

	void draw()
	{
		for (auto& column : m_field)
		{
			for (auto& cell : column)
			{
				cell = 0;
			}
		}
		std::for_each(std::begin(particles), std::end(particles), [this]( auto& particle)
			{
				++m_field[particle.x() / squares_size][particle.y() / squares_size];
			});

		sf::RectangleShape big_rect(sf::Vector2f(squares_size, squares_size));
		for (auto i = 0; i < m_squares_in_column; ++i)
		{
			for (auto j = 0; j < m_squares_in_column; ++j)
			{
				sf::Color color;
				const std::size_t particles_num = m_field[i][j];
				if (!particles_num)
				{
					color = sf::Color::White;
				}
				else
				{
					auto saturation = particles_num / change_num;
					saturation = (saturation > colors_num - 1 ? colors_num - 1 : saturation);
					color = m_colors[saturation];
				}
				big_rect.setPosition(static_cast <float> (i * squares_size), static_cast <float> (j * squares_size));
				big_rect.setFillColor(color);
				m_window.draw(big_rect);
			}
		}
		sf::RectangleShape small_rect(sf::Vector2f(1.0f, 1.0f));
		std::for_each(std::begin(particles), std::end(particles), [this, &small_rect](Particle& particle)
			{
				small_rect.setPosition(static_cast<float> (particle.x()), static_cast<float>(particle.y()));
				small_rect.setFillColor(m_particle_color);
				m_window.draw(small_rect);
			});
		m_window.display();
	}

private:
	inline static const std::size_t m_width = 1000;
	inline static const std::size_t m_height = 1000;
	inline static const std::size_t m_particles_num = 10000;
	inline static const std::size_t change_num = 5;
	inline static const std::size_t m_squares_in_column = 20;
	inline static const std::size_t squares_size = 50;
	inline static const std::size_t colors_num = 10;
	const std::string m_window_name = "Brownian motion";
	const sf::Color m_particle_color = { 255, 255, 255 };
	const std::array<sf::Color, colors_num> m_colors =
	{ sf::Color(0, 0, 200), sf::Color(0, 0, 180), sf::Color(0, 0, 160), sf::Color(0, 0, 140), sf::Color(0, 0, 120), 
		sf::Color(0, 0, 100), sf::Color(0, 0, 80), sf::Color(0, 0, 60), sf::Color(0, 0, 40), sf::Color(0, 0, 20) };
	std::vector<Particle> particles;
	std::vector<std::vector<std::size_t>> m_field;
	sf::RenderWindow m_window;




};

int main()
{
	Visualizer visualizer;
	system("pause");
	return 0;
}