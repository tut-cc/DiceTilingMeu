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
  return dst.str();
}

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
