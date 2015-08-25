#include "SimpleProblem.hpp"
#include <iostream>

template <class F, class S>
SimpleProblem<F, S>::SimpleProblem() : field(new F()), stones([](){
  std::vector<std::shared_ptr<Stone>> list;
  int num;
  std::cin >> num;
  for(int i = 0; i < num; ++i) {
    std::shared_ptr<Stone> stone(new S());
  }
  return list;
}()) {}


template <class F, class S>
std::unique_ptr<Field> SimpleProblem<F, S>::get_field()
{
  return std::move(field -> clone());
}

template <class F, class S>
std::vector<std::weak_ptr<Stone>> SimpleProblem<F, S>::get_stones()
{
  std::vector<std::weak_ptr<Stone>> list;
  for(auto stone : stones) {
    list.push_back(stone);
  }
  return list;
}

