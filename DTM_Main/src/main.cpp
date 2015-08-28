#include "Beamalgo.hpp"
#include "SimpleField.hpp"
#include "SimpleStone.hpp"
#include "SimpleProblem.hpp"
#include "SampleProblem.hpp"
#include <memory>

int main()
{
  std::shared_ptr<Problem> prob(new SampleProblem());
  std::unique_ptr<Algorithm> solver(new Beamalgo<SimpleField, SimpleStone>(prob));
  solver -> solve();
  std::string a;
  std::cin >> a;
  return 0;
}

