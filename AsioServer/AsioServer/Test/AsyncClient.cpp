#include "AsyncClient.h"

size_t AsyncClient::read_complete(const error_code& err, size_t bytes)
{
	return std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes ? 0 : 1;
}

void AsyncClient::do_read()
{
	async_read(sock_, buffer(read_buffer_), boost::bind(&AsyncClient::read_complete, this->shared_from_this(), _1, _2),
											boost::bind(&AsyncClient::on_read, this->shared_from_this(), _1, _2));
}

void AsyncClient::on_read(const error_code& err, size_t bytes)
{
	if (!err)
	{
		std::string msg(read_buffer_, bytes - 1);
		std::cout << "server echo: " << msg << " : " << (msg == message_ ? "OK" : "FAIL") << std::endl;
	}
	stop();
}

void AsyncClient::do_write(const std::string& msg)
{
	if (!started_)
		return;

	std::copy(msg.begin(), msg.end(), write_buffer_);

	std::cout << " ready to write msg: " << msg << std::endl;

	sock_.async_write_some(buffer(write_buffer_, msg.size()), boost::bind(&AsyncClient::on_write, shared_from_this(), _1, _2));
}

void AsyncClient::on_write(const error_code& err, size_t bytes)
{
	std::cout << "on write" << std::endl;
	do_read();
}

void AsyncClient::on_connect(const error_code& err)
{
	std::cout << "connect finish! current msg: " << message_ << std::endl;
	if (!err)
	{
		do_write((message_ + "\n"));
	}
	else
	{
		stop();
	}
}

int main()
{
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8000);
	char* messages[] = { "msg1", "msg2", "msg3" };

	for (auto* msg : messages)
	{
		if (!msg)
			continue;

		AsyncClient::start(ep, msg);
		boost::this_thread::sleep(boost::posix_time::millisec(100));
	}

	service.run();
	system("pause");

	return 0;
}
