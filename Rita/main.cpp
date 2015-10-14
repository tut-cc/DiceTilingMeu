#include "Ritalgo.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include "ProductionProblem.hpp"
#include "Core.hpp"
#include <memory>

int main(const int argc, const char **argv)
{
  if (argc == 1) {
    std::shared_ptr<Problem> prob(new SimpleProblem());
    std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob));
    rita->solve();
  }
  else {
    std::unique_ptr<Core> core(new Core(argv[1]));
    const std::string probs = core->get();
    std::shared_ptr<Problem> prob(new ProductionProblem(probs));
    std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob, std::move(core)));
    rita->solve();
  }
  return 0;
}