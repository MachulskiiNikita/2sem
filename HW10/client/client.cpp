#include <iostream>
#include <boost/asio.hpp>
#include <thread>

class Chat
{
public:
	Chat() : ip_address("127.0.0.1"), port(3333), exit_flag(false), socket(io_service, endpoint.protocol()),
		final_point(boost::asio::ip::address::from_string(ip_address), port), endpoint(boost::asio::ip::address::from_string(ip_address), port),
		io_service(), reader(std::thread([this]() {read_message(socket); }))
	{
		std::cout << "To leave the chat write QUIT\nInput your name";
		std::cin >> m_name;
		socket.connect(final_point);
		write_message(socket, cref(m_name));
	}


	~Chat()
	{
		reader.join();
	}
public:
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
private:
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

private:
	std::string ip_address;
	int port;
	std::string m_name;
	std::atomic < bool > exit_flag;
	boost::asio::ip::tcp::endpoint endpoint;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket;
	boost::asio::ip::tcp::endpoint final_point;
	std::thread reader;
};



int main(int argc, char* argv[])
{
	Chat chat();

	return EXIT_SUCCESS;
}