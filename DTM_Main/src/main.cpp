#include "Beamalgo.hpp"
#include "Ritalgo.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include <memory>

int main()
{
  std::shared_ptr<Problem> prob(new SimpleProblem());
  std::unique_ptr<Algorithm> solver(new Beamalgo<ExtendedField, ExtendedStone>(prob));
//  std::unique_ptr<Algorithm> rita(new Ritalgo<SimpleField, SimpleStone>(prob));
  solver -> solve();
//  rita -> solve();
  return 0;
}

