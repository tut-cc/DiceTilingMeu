#include "../../Algorithm/include/Ritalgo.h"
#include <iostream>

int main()
{
  auto solver = new Ritalgo();
  solver -> solve();
  delete solver;
  return 0;
}

