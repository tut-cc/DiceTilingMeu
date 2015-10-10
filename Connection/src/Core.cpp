#include "Core.hpp"

Core::Core(asio::io_service & io_service, const std::string & host)
  : io_service_(io_service),
  socket_(io_service),
  host_(host)
{
}

/**
　　lllllllllllllllllll,,,　　 　 ,,,llllllllllllll,,,　 　 　,,,,llllllllllllll,,,　 　　 ,,,,llllllllllllllll,,, 　 ,llllllllllllllllllllllllllllll'
　 llllllll'''''''''llllll　　 ,,llllllll'''''''lllll,　　 ,llllllll''''''''llllllll,　 　,lllllll''''''''''''lll''　　'''''''''''llllllll''''''''''''
　,llllllll 　 ,,lllllll' 　 ,llllllll' 　 　'llllllll,　 llllllll'　 　  'llllllll, 　 lllllllll,,,,,　　 　　　　            　,llllllll
　llllllllllllllllllllll,,　llllllll　　   　lllllll 　llllllll　　　    lllllllll　 　'''llllllllllllllll,,,　　 　         　 llllllll'
　lllllll　 　''lllllll,　 lllllllll 　 　 llllllll'　 llllllll 　 　   ,llllllll　 　　　 ''''''llllllllll 　 　          　 llllllll
　llllll 　　,,llllllll 　 lllllllll, 　 ,,lllllll'' 　 lllllllll, 　 ,,lllllll''　　 ,,,,,,,　　 ,,llllllll 　 　      　 ,llllllll
,lllllllllllllllllll''' 　 　'llllllllllllll111'' 　 　 ''llllllllllllll1l''' 　　 'llllllllllllllllllllll''' 　   　 　 llllllll'
''''''''''''''''　 　 　 　 　 ''''''''''　 　 　　 　 '''''''''' 　　　　　　'''''''''''' 　　　　　　 ''''''''
 */
std::string Core::get()
{
  try {
    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(host_, "http");
    tcp::resolver::iterator iterator = resolver.resolve(query);

    boost::asio::connect(socket_, iterator);
  }
  catch (std::exception e) {
    std::cerr << e.what() << std::endl;
  }

  boost::asio::streambuf request;
  std::ostream rs(&request);
  auto constexpr ENDL = "\r\n";

  rs << "GET " << "/quest HTTP/1.0" << ENDL;
  rs << "Host: " << host_ << ENDL;
  rs << "Accept: */*" << ENDL;
  rs << "Connection: close" << ENDL << ENDL;

  boost::asio::write(socket_, request);

  std::stringstream ss;
  try
  {
    for (;;)
    {
      char data[19749];

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

/*
　　　  ,,,lllllllllllllllll,,,,　　,lllllllll　　　,lllllllll　　,lllllllllllllllllllllll,,　 　　   lllllllll
   　　,llllllllllll''''''llllll''lllllllll　　　,lllllllll　　,lllllllll'''''''lllllllllll,　 　 lllllllll
   　,lllllllll'' 　　　 　 　 llllllllll 　 　,lllllllll　　,lllllllll'　 　    llllllllll　　 ,lllllllll
   　llllllll' 　　 　 　 　 ,lllllllll' 　 　lllllllll' 　 ,lllllllll,,,,,,,,,,,lllllllll'　 ,lllllllll'
  　lllllll 　 　 　 　 　 lllllllll' 　 　llllllllll 　 ,llllllllllllllllllllllll''' 　 　 lllllllll'
  'llllllllll,　　　　　　,llllllllll　　 ,lllllllll' 　 ,lllllllll'　 llllllllll, 　 　　lllllllll
　lllllllllll,,,,,,,,,lllllllllllll,,,,,,llllllll' 　　llllllllll 　 'llllllllll, 　 　,llllllllll,,,,,,,,,,,,,,,,,
　 ''lllllllllllllllllll''llllllllllllllllll''''　 　 llllllllll 　　 'lllllllll,　　 lllllllllllllllllllllllllll'
   　　 　'''''''' 　 　 　 　 　''''''''
 */
void Core::submit(int score, int stones, const std::string& filename = "answer.latest.txt")
{
  std::stringstream ss;
  ss << "curl http://" << host_ << "/update "
    << R"(--form-string "score=)"  << score    << R"(" )"
    << R"(--form-string "stones=)" << stones   << R"(" )"
    << R"(-F "answer=@)"           << filename << R"(")";
  std::cerr << ss.str() << std::endl;
  std::system(ss.str().c_str());
  std::cerr << "POST done" << std::endl;
}
