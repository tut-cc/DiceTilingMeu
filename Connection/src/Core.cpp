#include "Core.hpp"
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>

Core::Core(asio::io_service & io_service, const std::string & host)
  : io_service_(io_service),
  socket_(io_service),
  host_(host)
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
  std::string fn = boost::posix_time::to_iso_string(time) + ".txt";

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
    dst << line;
  }

  std::cerr << "GET done" << std::endl;

  return dst.str();
}

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
