#include <set>
#include <vector>
#include <algorithm>
#include "Timer.hpp"

std::vector < int > generate_random_numbers(int N)
{
	std::vector < int > vector;
	srand(time(NULL));
	for (int i = 0; i < N; i++)
	{
		vector.push_back(rand());
	}
	return vector;
}


void fill_set(std::multiset < int > set, std::vector < int > random_numbers)
{
	for (int i = 0; i < random_numbers.size(); i++)
	{
		set.insert(random_numbers[i]);
	}
}


void fill_vector(std::vector < int > vector, std::vector < int > random_numbers)
{
	for (int i = 0; i < random_numbers.size(); i++)
	{
		vector.push_back(random_numbers[i]);
	}
}


void make_measurements(int number_of_measurements = 100, int quantity_of_numbers = 100000)
{
	std::vector < int > random_numbers = generate_random_numbers(quantity_of_numbers);
	std::multiset < int > set;
	std::vector < int > vector;

	Timer set_timer;
	set_timer.stop();
	Timer vector_timer;
	vector_timer.stop();
	
	for (int i = 0; i < number_of_measurements; i++)
	{
		std::vector < int > random_numbers = generate_random_numbers(quantity_of_numbers);
		
		set_timer.start();
		fill_set(set, random_numbers);
		set_timer.stop();
		
		vector_timer.start();
		fill_vector(vector, random_numbers);
		std::sort(std::begin(vector), std::end(vector));
		vector_timer.stop();
	}

	double set_result = std::chrono::duration <double> (set_timer.get_interval()/ number_of_measurements).count();
	double vector_result = std::chrono::duration <double> (vector_timer.get_interval() / number_of_measurements).count();
	std::cout << "set time = " << set_result << "\nvector time = " << vector_result;
}


int main()
{
	make_measurements();
}