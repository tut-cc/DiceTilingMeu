#include "Beamalgo.hpp"
#include "Ritalgo.hpp"
#include "meu.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "BoostBitField.hpp"
#include "Client.hpp"
#include "Core.hpp"
#include <memory>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const int max_length = 1024;

int main()
{

  //std::shared_ptr<Problem> prob(new SimpleProblem());
  // std::unique_ptr<Algorithm> solver(new Beamalgo<ExtendedField, ExtendedStone>(prob));
  // std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob));
  // solver -> solve();
  // rita -> solve();
  //std::unique_ptr<Algorithm> meu(new Meu<SimpleField, SimpleStone>(prob));
  //meu->solve();

  // std::unique_ptr<FieldEX<boost::multiprecision::uint1024_t>> fx(new BoostBitField(prob->get_field_str()));

  boost::asio::io_service ios;
  // test DONE
  //Client cl(ios, "172.26.32.189", 65534);
  //cl.accept();
  //auto str = cl.receive();
  //std::cerr << str << std::endl;

  //Core core(ios, "testform26.procon-online.net", "0123456789abcdef", 1);
  //auto probs = core.get();
  Core core2(ios, "testform26.procon-online.net", "0123456789abcdef", 1);
  std::stringstream ss;
  for (int i = 0; i < 36; ++i) {
    ss << std::endl;
  }
  core2.submit(ss.str());

  return 0;
}

