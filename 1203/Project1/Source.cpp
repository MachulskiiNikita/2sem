#include <iostream>

int main()
{
	//int x;

	//while (std::cin >> x)
	//{
	//	std::cout << x;
	//}

	//std::cin.clear();

	//while (std::cin >> x)
	//{
	//	std::cout << x;
	//}

	std::ofstream fout("file.txt");

	fout << "HELLO";

	return 0;
}