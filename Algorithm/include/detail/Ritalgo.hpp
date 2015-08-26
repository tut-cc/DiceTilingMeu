#pragma once
#include "Ritalgo.h"
#include <iostream>

template <class F, class S>
Ritalgo<F, S>::Ritalgo(std::shared_ptr<Problem> p)
{
  field = std::move(std::unique_ptr<Field>(new F(p -> get_field_str())));
  for( auto&& s : p -> get_stones_str() ) {
    stones.push_back(std::shared_ptr<Stone>(new S(s)));
  }
}

template <class F, class S>
void Ritalgo<F, S>::solve()
{
  std::cout << "チェストー" << std::endl;
}

