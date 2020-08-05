#include <iostream>
#include "boost/thread.hpp"
#include "boost/bind.hpp"
#include "boost/asio.hpp"

using namespace boost::asio;
using boost::system::error_code;

io_service service;
ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8000);

size_t read_complete(char* buff, const error_code& error, size_t bytes)
{
	if (error)
		return 0;

	return (std::find(buff, buff + bytes, '\n') < buff + bytes) ? 0 : 1;
}

void sync_echo(std::string msg)
{
	msg += "\n";
	ip::tcp::socket sock(service);
	sock.connect(ep);
	sock.write_some(buffer(msg));

	char buff[1024];
	int bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));  // read data
	
	std::string copy(buff, bytes - 1);
	msg = msg.substr(0, msg.size() - 1);
	std::cout << "server echo: " << msg << " : " << (copy == msg ? "OK" : "FAIL") << std::endl;

	sock.close();
}

int main()
{
	char* message[] = {"message1", "message2", "message3", "message4", "message5"};
	boost::thread_group threads;

	for (auto* msg : message)
	{
		if (!msg)
			continue;

		threads.create_thread(boost::bind(sync_echo, msg));
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}

	threads.join_all();

	system("pause");

	return 0;
}
