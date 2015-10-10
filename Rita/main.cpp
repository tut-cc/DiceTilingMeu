#include "Beamalgo.hpp"
#include "Ritalgo.hpp"
#include "meu.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include "ProductionProblem.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "BoostBitField.hpp"
#include "Client.hpp"
#include "Core.hpp"
#include <memory>
#include <boost/asio.hpp>

int main(const int argc, const char **argv)
{
  if (argc == 1) {
    std::shared_ptr<Problem> prob(new SimpleProblem());
    std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob));
    rita->solve();
  }
  else {
    boost::asio::io_service ios;
    std::unique_ptr<Core> core(new Core(ios, argv[1]));
    auto probs = core.get();
    std::shared_ptr<Problem> prob(new ProductionProblem(probs));
    std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob, std::move(core)));
    rita->solve();
  }
  return 0;
}

