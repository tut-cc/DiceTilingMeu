#include "Core.hpp"

Core::Core(asio::io_service & io_service, const std::string & host, const std::string & token, int num)
  : io_service_(io_service),
  socket_(io_service),
  host_(host),
  token_(token),
  num_(num)
{
  tcp::resolver resolver(io_service_);
  tcp::resolver::query query(host_, "http");
  tcp::resolver::iterator iterator = resolver.resolve(query);

  boost::asio::connect(socket_, iterator);
}

std::string Core::get()
{
  boost::asio::streambuf request;
  std::ostream rs(&request);
  auto constexpr ENDL = "\r\n";

  rs << "GET " << "/quest" << num_ << ".txt?token=" << token_ << " HTTP/1.0" << ENDL;
  rs << "Host: " << host_ << ENDL;
  rs << "Accept: */*" << ENDL;
  rs << "Connection: close" << ENDL << ENDL;

  boost::asio::write(socket_, request);

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

  std::string str;
  std::stringstream prob;
  while (std::getline(ss, str)) {
    if (str.find_first_of(ENDL) == 0) {
      break;
    }
  }
  while (std::getline(ss, str)) {
    prob << str;
  }
  return prob.str();
}

void Core::submit(const std::string& answer)
{
  auto constexpr BOUNDARY = "cce6735153bf14e47e999e68bb183e70a1fa7fc89722fc1efdf03a917340";
  auto constexpr ENDL     = "\r\n";

  boost::asio::streambuf request;
  std::ostream rs(&request);

  rs << "POST " << "/answer" << " HTTP/1.1" << ENDL;
  rs << "Host: " << host_ << ENDL;
  rs << "Content-Type: multipart/form-data; boundary=" << BOUNDARY << ENDL;
  rs << ENDL;
  rs << "--" << BOUNDARY << ENDL;
  rs << R"(Content-Disposition: form-data; name="token")" << ENDL;
  rs << ENDL;
  rs << token_ << ENDL;
  rs << "--" << BOUNDARY << ENDL;
  rs << R"(Content-Disposition: form-data; name="answer"; filename="answer.txt")" << ENDL;
  rs << "Content-Type: text/plain" << ENDL;
  rs << ENDL;
  rs << answer << ENDL;
  rs << "--" << BOUNDARY << "--" << ENDL << ENDL;

  const std::string result = boost::asio::buffer_cast<const char*>(request.data());
  std::cerr << result << std::endl;

  boost::asio::write(socket_, request);

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

  std::cerr << ss.str() << std::endl;

  std::cerr << "POST done" << std::endl;
}
