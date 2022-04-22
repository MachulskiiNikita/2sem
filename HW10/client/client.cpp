#include <iostream>
#include <boost/asio.hpp>
#include <thread>


void read_message(boost::asio::ip::tcp::socket& sock)
{
	while (true) {
		boost::asio::streambuf buffer;
		std::string message;
		boost::asio::read_until(sock, buffer, ';');
		std::istream input_stream(&buffer);
		getline(input_stream, message, ';');
		std::cout << message << std::endl;
	}
}

void write_message(boost::asio::ip::tcp::socket& sock, const std::string& name)
{
	std::string message;
	std::cin >> message;
	std::string output_message;

	while (message != "QUIT")
	{
		output_message = "(" + name + "):   " + message + ';';
		auto buf = boost::asio::buffer(output_message);
		boost::asio::write(sock, buf);
		std::cin >> message;
	}

	output_message = "(" + name + ")" + " has left the chat";
	auto buf = boost::asio::buffer(output_message);
	boost::asio::write(sock, buf);
}

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;
	std::string ip_address = "127.0.0.1";
	std::string name;
	std::cout << "To leave the chat write QUIT\nInput your name:" << std::endl;
	std::cin >> name;
	boost::asio::ip::tcp::endpoint final_point(boost::asio::ip::address::from_string(ip_address), 42);
	boost::asio::ip::tcp::socket socket(io_service, final_point.protocol());
	socket.connect(final_point);

	std::thread reader = std::thread(read_message, ref(socket));
	write_message(socket, cref(name));
	reader.join();

	return EXIT_SUCCESS;
}