#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

class Client {
  asio::io_service& io_service_;
  tcp::socket socket_;
  std::string addr_;
  int port_;

public:
  Client(asio::io_service&, const std::string&, int);
  Client() = delete;
  Client(const Client &) = delete;
  Client(Client &&) = delete;
  ~Client() = default;

  void connect();
  void accept();
  std::string receive();
  void send(const std::string);
};


// made from http://boostjp.github.io/tips/network/tcp.html
