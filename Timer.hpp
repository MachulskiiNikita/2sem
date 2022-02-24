#pragma once
#include <iostream>
#include <chrono>


class Timer
{
public:
	using clock_t = std::chrono::steady_clock;
	using time_point_t = clock_t::time_point;

	Timer() : m_begin(std::chrono::steady_clock::now()), interval(0), flag(true)
	{}


	void stop()
	{
		if (!flag)
			{
				std::cout << "Timer already stopped" << std::endl;
			}
		else
		{
			flag = false;
			interval += std::chrono::steady_clock::now() - m_begin;
		}
	}

	void start()
	{
		if (flag)
			{
				std::cout << "Timer already started!" << std::endl;
			}
		else
		{
			flag = true;
			m_begin = std::chrono::steady_clock::now();
		}
	}

	void reset()
	{
		interval = std::chrono::microseconds::zero();
		m_begin = std::chrono::steady_clock::now();
	}

	std::chrono::duration < double > get_interval()
	{
		return interval;
	}
private:
	std::chrono::steady_clock::time_point m_begin;
	std::chrono::duration < double > interval;
	bool flag;
};
