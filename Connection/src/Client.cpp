#include "Client.hpp"

Client::Client(asio::io_service& io_service, const std::string& addr, int port)
  : io_service_(io_service),
  socket_(io_service),
  addr_(addr),
  port_(port)
{}

void Client::connect()
{
  boost::system::error_code error;
  socket_.connect(tcp::endpoint(asio::ip::address::from_string(addr_), port_), error);

  std::cerr << "[" << addr_ << "]";
  if (error) {
    std::cerr << "connect failed : " << error.message() << std::endl;
  }
  else {
    std::cerr << "connected" << std::endl;
  }
}

void Client::accept()
{
  tcp::acceptor acc(io_service_, tcp::endpoint(tcp::v4(), port_));
  tcp::socket socket(io_service_);

  boost::system::error_code error;
  acc.accept(socket, error);

  std::cerr << "[" << addr_ << "]";
  if (error) {
    std::cerr << "accept failed: " << error.message() << std::endl;
  }
  else {
    std::cerr << "accept correct!" << std::endl;
  }

}

std::string Client::receive()
{
  asio::streambuf receive_buffer;
  boost::system::error_code error;
  asio::read(socket_, receive_buffer, asio::transfer_all(), error);

  if (error && error != asio::error::eof) {
    std::cerr << "[" << addr_ << "]" << "receive failed: " << error.message() << std::endl;
  }
  const char* data = asio::buffer_cast<const char*>(receive_buffer.data());
  std::string str(data);

  return str;
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
