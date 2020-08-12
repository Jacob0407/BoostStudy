#include <iostream>
#include <string>
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/core/noncopyable.hpp"
#include "boost/thread.hpp"
#include<boost/date_time/gregorian/gregorian.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service service;


class AsyncClient : public boost::enable_shared_from_this<AsyncClient>, boost::noncopyable
{
public:
	static boost::shared_ptr<AsyncClient> start(ip::tcp::endpoint ep, const std::string& msg)
	{
		boost::shared_ptr<AsyncClient> client = boost::shared_ptr<AsyncClient>(new AsyncClient(msg));
		client->start(ep);
		return client;
	}

	void do_write(const std::string& msg);
	void on_write(const error_code& err, size_t bytes);
	size_t read_complete(const error_code& err, size_t bytes);
	void do_read();
	void on_read(const error_code& err, size_t bytes);

	inline ip::tcp::socket& sock() { return sock_; }
	inline void stop()
	{
		if (!started_)
			return;

		started_ = false;
		sock_.close();
	}
	bool started() { return started_; }

private:
	AsyncClient() : sock_(service), started_(false) {};
	AsyncClient(const std::string& msg) : sock_(service), started_(false), message_(msg) {};
	inline void start(ip::tcp::endpoint ep)
	{
		started_ = true;
		sock_.async_connect(ep, boost::bind(&AsyncClient::on_connect, shared_from_this(), _1));
	}
	void on_connect(const error_code& err);

private:
	ip::tcp::socket sock_;
	char read_buffer_[1024];
	char write_buffer_[1024];
	bool started_ = false;
	std::string message_;
};