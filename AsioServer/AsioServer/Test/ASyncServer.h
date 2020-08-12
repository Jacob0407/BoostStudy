#include <iostream>
#include <string>
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/core/noncopyable.hpp"
#include<boost/date_time/gregorian/gregorian.hpp>

using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service service;


/*
	async server
*/

class ASyncServer : public boost::enable_shared_from_this<ASyncServer>, boost::noncopyable
{
	ASyncServer() : sock_(service), started_(false) {}
public:

	void start();
	void stop();
	static boost::shared_ptr<ASyncServer> new_()
	{
		return boost::shared_ptr<ASyncServer>(new ASyncServer);
	}

	inline ip::tcp::socket& sock() { return sock_; }

private:
	
	void on_read(const error_code& err, size_t bytes);
	void on_write(const error_code& err, size_t bytes);
	void do_read();
	void do_write(const std::string& msg);
	size_t read_complete(const error_code& err, size_t bytes);

private:
	ip::tcp::socket sock_;
	char read_buffer_[1024];
	char write_buffer_[1024];
	bool started_ = false;
};