#include "Client.hpp"

Client::Client(asio::io_service& io_service, const std::string& addr, int port)
  : io_service_(io_service),
  socket_(io_service),
  addr_(addr),
  port_(port)
{}

void Client::connect()
{
  tcp::resolver resolver(io_service_);
  tcp::resolver::query query(tcp::v4(), addr_, std::to_string(port_));
  tcp::resolver::iterator iterator = resolver.resolve(query);

  boost::asio::connect(socket_, iterator);
}

void Client::accept()
{
  tcp::acceptor a(io_service_, tcp::endpoint(tcp::v4(), port_));
  a.accept(socket_);
}

std::string Client::receive()
{
  std::stringstream ss;
  try
  {
    for (;;)
    {
      char data[8192];

      boost::system::error_code error;
      size_t length = socket_.read_some(boost::asio::buffer(data), error);
      ss.write(data, length);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }

  return ss.str();
}

void Client::send(const std::string msg)
{
  boost::system::error_code error;
  asio::write(socket_, asio::buffer(msg), error);

  std::cerr << "[" << addr_ << "]";
  if (error) {
    std::cerr << "send failed: " << error.message() << std::endl;
  }
  else {
    std::cerr << "send correct!" << std::endl;
  }
}
