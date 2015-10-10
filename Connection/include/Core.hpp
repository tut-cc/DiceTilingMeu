#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

class Core {
  asio::io_service& io_service_;
  tcp::socket socket_;
  std::string host_;

public:
  Core(asio::io_service&, const std::string&);
  Core() = delete;
  Core(const Core &) = delete;
  Core(Core &&) = delete;
  ~Core() = default;

  virtual std::string get(bool flag=false) final;
  void submit(int, int, const std::string &);
};