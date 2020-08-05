#include <iostream>
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service service;

size_t read_complete(char* buff, const error_code& error, size_t bytes)
{
	if (error)
		return 0;

	return (std::find(buff, buff + bytes, '\n') < buff + bytes) ? 0 : 1;
}

void handle_connections()
{
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8000));
	char buff[1024];

	std::cout << "sync server begin running..." << std::endl;
	
	while (true)
	{
		ip::tcp::socket sock(service);
		acceptor.accept(sock);
		int bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));

		std::string msg(buff, bytes);
		std::cout << "from ip: " << sock.remote_endpoint().address() <<  " receive msg: " << msg << std::endl;
		sock.write_some(buffer(msg));
		sock.close();
	}
}

int main()
{
	handle_connections();

	return 0;
}
