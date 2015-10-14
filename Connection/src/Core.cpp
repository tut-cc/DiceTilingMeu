#include "Core.hpp"
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

Core::Core(const std::string & host) : host_(host)
{
}

/**
    lllllllllllllllllll,,,        ,,,llllllllllllll,,,        ,,,,llllllllllllll,,,        ,,,,llllllllllllllll,,,    ,llllllllllllllllllllllllllllll'
   llllllll'''''''''llllll     ,,llllllll'''''''lllll,     ,llllllll''''''''llllllll,     ,lllllll''''''''''''lll''    '''''''''''llllllll''''''''''''
  ,llllllll    ,,lllllll'    ,llllllll'      'llllllll,   llllllll'       'llllllll,    lllllllll,,,,,                         ,llllllll
  llllllllllllllllllllll,,  llllllll         lllllll   llllllll          lllllllll     '''llllllllllllllll,,,                 llllllll'
  lllllll     ''lllllll,   lllllllll       llllllll'   llllllll         ,llllllll          ''''''llllllllll                 llllllll
 llllll     ,,llllllll    lllllllll,    ,,lllllll''    lllllllll,    ,,lllllll''     ,,,,,,,     ,,llllllll             ,llllllll
,lllllllllllllllllll'''      'llllllllllllll111''       ''llllllllllllll1l'''      'llllllllllllllllllllll'''          llllllll'
''''''''''''''''               ''''''''''                     ''''''''''                  ''''''''''''                ''''''''
 */
std::string Core::get(bool flag)
{
  boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
  std::string fn = "tmp_prob/" + boost::posix_time::to_iso_string(time) + ".txt";

  std::stringstream ss;
  if (flag) {
    std::ifstream ifs("settings.txt");
    std::string url, token;
    int num;
    ifs >> url;
    ifs >> token;
    ifs >> num;
    ss << "curl http://" << url << "/quest" << num << ".txt?token=" << token;
  }
  else {
    ss << "curl http://" << host_ << "/problem > " << fn;
  }
  std::cerr << ss.str() << std::endl;
  std::system(ss.str().c_str());

  std::ifstream ifs(fn);
  std::stringstream dst;
  while (!ifs.eof()) {
    std::string line;
    std::getline(ifs, line);
    dst << line << std::endl;
  }
<<<<<<< HEAD
  return prob.str();
}

void Core::submit(const std::string& answer)
{
  auto constexpr BOUNDARY = "--cce6735153bf14e47e999e68bb183e70a1fa7fc89722fc1efdf03a917340";
  auto constexpr ENDL     = "\r\n";
=======
>>>>>>> 1183e493edd48b87909315f07f677e696073fd56

  std::cerr << "GET done" << std::endl;

<<<<<<< HEAD
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
  rs << ENDL;
  rs << "--" << BOUNDARY << "--" << ENDL << ENDL;

  const std::string result = boost::asio::buffer_cast<const char*>(request.data());
  std::cerr << result << std::endl;

  boost::asio::write(socket_, request);
=======
  return dst.str();
}
>>>>>>> 1183e493edd48b87909315f07f677e696073fd56

/*
        ,,,lllllllllllllllll,,,,    ,lllllllll      ,lllllllll    ,lllllllllllllllllllllll,,          lllllllll
       ,llllllllllll''''''llllll''lllllllll      ,lllllllll    ,lllllllll'''''''lllllllllll,      lllllllll
     ,lllllllll''              llllllllll      ,lllllllll    ,lllllllll'         llllllllll     ,lllllllll
     llllllll'               ,lllllllll'      lllllllll'    ,lllllllll,,,,,,,,,,,lllllllll'   ,lllllllll'
    lllllll                lllllllll'      llllllllll    ,llllllllllllllllllllllll'''       lllllllll'
  'llllllllll,            ,llllllllll     ,lllllllll'    ,lllllllll'   llllllllll,        lllllllll
  lllllllllll,,,,,,,,,lllllllllllll,,,,,,llllllll'     llllllllll    'llllllllll,      ,llllllllll,,,,,,,,,,,,,,,,,
   ''lllllllllllllllllll''llllllllllllllllll''''      llllllllll      'lllllllll,     lllllllllllllllllllllllllll'
          ''''''''               ''''''''
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
