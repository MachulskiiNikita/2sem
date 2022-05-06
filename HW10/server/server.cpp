#include <iostream>
#include <boost/asio.hpp>
#include <thread>


class Chat
{
public:
	Chat(): m_name("Server"), port(3333), io_service(), endpoint(boost::asio::ip::address_v4::any(), port), exit_flag(false),
		socket(io_service), acceptor(io_service, endpoint.protocol()), size(30), reader (std::thread([this]() {read_message(socket); }))
	{
		std::cout << "To leave the chat write QUIT" << std::endl;
		acceptor.bind(endpoint);
		acceptor.listen(size);
		acceptor.accept(socket);
		auto reader = std::thread([this]() {read_message(socket); });
		write_message(socket, cref(m_name));
	};
	~Chat()
	{
		reader.join();
	}
public:
	void read_message(boost::asio::ip::tcp::socket& socket)
	{
		while (true) {
			boost::asio::streambuf buffer;
			std::string message;
			boost::asio::read_until(socket, buffer, ';');
			std::istream input_stream(&buffer);
			getline(input_stream, message, ';');
			std::cout << message << std::endl;
		}
	}
private:
	void write_message(boost::asio::ip::tcp::socket& socket, const std::string& name)
	{
		std::string message;
		std::cin >> message;
		std::string output_message;

		while (message != "QUIT")
		{
			output_message = "(" + name + "):   " + message + ';';
			auto buf = boost::asio::buffer(output_message);
			boost::asio::write(socket, buf);
			std::cin >> message;
		}

		output_message = "(" + name + ")" + " has left the chat";
		auto buf = boost::asio::buffer(output_message);
		boost::asio::write(socket, buf);
	}


private:
	int port;
	std::string m_name;
	const std::size_t size;
	boost::asio::ip::tcp::endpoint endpoint;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;
	std::atomic < bool > exit_flag;
	std::thread reader;
};





int main(int argc, char* argv[])
{
	Chat chat;
	
	return EXIT_SUCCESS;
}