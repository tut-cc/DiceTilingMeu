#include "Ritalgo.h"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include <memory>

int main()
{
  std::shared_ptr<Problem> prob(new SimpleProblem());
  std::unique_ptr<Ritalgo> solver(new Ritalgo(prob));
  solver -> solve();

  return 0;
}

