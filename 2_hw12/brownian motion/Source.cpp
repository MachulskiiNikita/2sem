#include <vector>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>

class Square
{
public:

	Square() noexcept = default;

	explicit Square(sf::Vector2f position_a, sf::Vector2f position_b) noexcept :
		m_position_a(position_a), m_position_b(position_b), m_count(0)
	{}

	~Square() noexcept = default;

public:
	const auto position_a() const noexcept
	{
		return m_position_a;
	}
	const auto position_b() const noexcept
	{
		return m_position_b;
	}
	const auto count() const noexcept
	{
		return m_count;
	}
	void add() noexcept
	{
		m_count++;
	}
	void clear() noexcept
	{
		m_count = 0;
	}

private:
	sf::Vector2f m_position_a;
	sf::Vector2f m_position_b;
	std::size_t m_count;
};

class Particle
{
public:

	Particle() noexcept = default;

	explicit Particle(sf::Vector2f position, sf::Vector2f speed) noexcept :
		m_position(position), m_speed(speed)
	{}

	~Particle() noexcept = default;

public:

	const auto position() const noexcept
	{
		return m_position;
	}
	const auto speed() const noexcept
	{
		return m_speed;
	}
	void set_x(float x) noexcept
	{
		m_position.x = x;
	}
	void set_y(float y) noexcept
	{
		m_position.y = y;
	}
	void set_speed(sf::Vector2f speed) noexcept
	{
		m_speed = speed;
	}

public:

	void move(float dt)
	{
		m_position += m_speed * dt;
	}

private:

	sf::Vector2f m_position;
	sf::Vector2f m_speed;
};

class System
{
public:

	using particle_t = std::shared_ptr < Particle >;
	using square_t = std::shared_ptr < Square >;


public:

	explicit System() noexcept :
		m_uid(0, m_N* m_size - 1), m_uid_s(-80, 80),
		m_application(sf::VideoMode(m_size* m_N, m_size* m_N), "Brownian motion"),
		m_time_point(std::chrono::steady_clock::now()),
		m_delay(1000000), m_timer(0),
		m_big_rect(sf::Vector2f(m_size, m_size)),
		m_small_rect(sf::Vector2f(1.0f, 1.0f))

	{
		for (auto j = 0U; j < m_particle_q; j++)
		{
			auto position = sf::Vector2f(m_uid(m_dre), m_uid(m_dre));
			auto speed = sf::Vector2f(m_uid_s(m_dre), m_uid_s(m_dre));
			m_particles.push_back(std::make_shared < Particle >(position, speed));
		}
		for (auto i = 0U; i < m_N; i++)
		{
			for (auto j = 0U; j < m_N; j++)
			{
				auto position_a = sf::Vector2f(i * m_size, j * m_size);
				auto position_b = sf::Vector2f((i + 1) * m_size, (j + 1) * m_size);
				m_squares.push_back(std::make_shared < Square >(position_a, position_b));
			}
		}
	}

	~System() noexcept = default;


public:
	void set_random_speed()
	{
		for (auto i = 0U; i < m_particle_q; i++)
		{
			auto speed = sf::Vector2f(m_uid_s(m_dre), m_uid_s(m_dre));
			m_particles[i]->set_speed(speed);
		}
	}


	void draw()
	{
		m_application.clear(sf::Color::Black);
		for (auto i = 0U; i < m_N * m_N; i++)
		{
			m_big_rect.setPosition(m_squares[i]->position_a().x, m_squares[i]->position_a().y);
			m_big_rect.setFillColor(sf::Color(0, 0, 255 * m_squares[i]->count() / (3 * m_particle_q / m_N / m_N)));
			m_application.draw(m_big_rect);
		}

		for (auto i = 0U; i < m_particle_q; i++)
		{
			m_small_rect.setPosition(m_particles[i]->position().x, m_particles[i]->position().y);
			m_small_rect.setFillColor(sf::Color::White);
			m_application.draw(m_small_rect);
		}
		m_application.display();
	}

	void run()
	{
		while (m_application.isOpen())
		{
			m_timer += std::chrono::duration_cast <std::chrono::microseconds> (std::chrono::steady_clock::now() - m_time_point);
			m_time_point = std::chrono::steady_clock::now();

			sf::Event event;

			while (m_application.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					m_application.close();
				}
			}

			if (m_timer > m_delay)
			{
				m_timer = std::chrono::microseconds(0);
				set_random_speed();
			}

			update();
			draw();
		}
	}

public:

	void update() const
	{
		for (auto i = 0U; i < m_N * m_N; i++)
		{
			m_squares[i]->clear();
		}

		for (auto i = 0U; i < std::size(m_particles); ++i)
		{
			m_particles[i]->move(0.01f);
			if (m_particles[i]->position().y < 0) m_particles[i]->set_y(m_max);
			if (m_particles[i]->position().x < 0) m_particles[i]->set_y(m_max);
			if (m_particles[i]->position().y > m_max) m_particles[i]->set_y(1);
			if (m_particles[i]->position().x > m_max) m_particles[i]->set_y(1);
			for (auto j = 0U; j < std::size(m_squares); ++j)
			{
				if (m_squares[j]->position_a().x < m_particles[i]->position().x &&
					m_particles[i]->position().x < m_squares[j]->position_b().x &&
					m_squares[j]->position_a().y < m_particles[i]->position().y &&
					m_particles[i]->position().y < m_squares[j]->position_b().y)
				{
					m_squares[j]->add();
					break;
				}
			}
		}
	}

private:

	std::vector < particle_t > m_particles;
	std::vector < square_t > m_squares;
	inline static const std::size_t m_N = 20;
	inline static const std::size_t m_size = 50;
	inline static const std::size_t m_particle_q = 3000;
	inline static const std::size_t m_max = m_N * m_size;
	std::default_random_engine m_dre;
	std::uniform_int_distribution <> m_uid;
	std::uniform_int_distribution <> m_uid_s;
	sf::RenderWindow m_application;
	std::chrono::steady_clock::time_point m_time_point;
	std::chrono::microseconds m_delay;
	std::chrono::microseconds m_timer;
	sf::RectangleShape m_big_rect;
	sf::RectangleShape m_small_rect;

};





int main()
{
	System system;
	system.run();
}