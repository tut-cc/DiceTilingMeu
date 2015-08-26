#include "Ritalgo.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include <memory>

int main()
{
  std::shared_ptr<Problem> prob(new SimpleProblem());
  std::unique_ptr<Algorithm> solver(new Ritalgo<SimpleField, SimpleStone>(prob));
  solver -> solve();

  return 0;
}

