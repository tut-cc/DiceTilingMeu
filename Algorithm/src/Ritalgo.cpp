#include "Ritalgo.h"
#include <iostream>

template <class F, class S>
Ritalgo::Ritalgo(std::shared_ptr<Problem<F, S>> p)
{
  field = p -> get_field();
  stones = p -> get_stones();
}

void Ritalgo::solve()
{
  std::cout << "チェストー" << std::endl;
}

