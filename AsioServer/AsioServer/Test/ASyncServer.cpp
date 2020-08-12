#include "ASyncServer.h"

void ASyncServer::start()
{
	started_ = true;
	do_read();
}

void ASyncServer::stop()
{
	if (!started_)
		return;

	started_ = false;
	sock_.close();
}

void ASyncServer::do_read()
{
	async_read(sock_, buffer(read_buffer_), boost::bind(&ASyncServer::read_complete, shared_from_this(), _1, _2), 
											boost::bind(&ASyncServer::on_read, shared_from_this(), _1, _2));
}

void ASyncServer::do_write(const std::string& msg)
{
	std::copy(msg.begin(), msg.end(), write_buffer_);
	sock_.async_write_some(buffer(write_buffer_, msg.size()), boost::bind(&ASyncServer::on_write, shared_from_this(), _1, _2));
}

void ASyncServer::on_read(const error_code& err, size_t bytes)
{
	if (!err)
	{
		std::string msg(read_buffer_, bytes);
		std::cout << " receive msg: " << msg << std::endl;
		std::cout << "ready write back" << std::endl;
		do_write((msg + "\n"));
	}
}

void ASyncServer::on_write(const error_code& err, size_t bytes)
{
	std::cout << "write back finished. ready to read." << std::endl;
	do_read();
}

size_t ASyncServer::read_complete(const error_code& err, size_t bytes)
{
	if (err)
		return 0;

	return (std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes) ? 0 : 1;
}


ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8000));

void handle_accept(boost::shared_ptr<ASyncServer> client, const error_code& err)
{
	if (!client)
		return;

	std::cout << "new connect, from " << client->sock().remote_endpoint().address() << std::endl;
	
	client->start();
	boost::shared_ptr<ASyncServer> new_client = ASyncServer::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}

int main()
{
	std::cout << "Async Server is running..." << std::endl;

	boost::shared_ptr<ASyncServer> client = ASyncServer::new_();
	acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));

	service.run();

	return 0;
}