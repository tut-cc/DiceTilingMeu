#include "SimpleProblem.hpp"
#include <string>
#include <iostream>

SimpleProblem::SimpleProblem() : field([](){
  std::vector<std::string> list;
  for (int i = 0; i < 32; ++i) {
    std::string str;
    std::cin >> str;
    list.push_back( str );
  }
  return list;
}()), stones([this](){
  std::vector<std::vector<std::string>> stones;
  std::cin >> num;
  for(int i = 0; i < num; ++i) {
    std::vector<std::string> stone;
    for(int i = 0; i < 8; ++i) {
      std::string str;
      std::cin >> str;
      stone.push_back(str);
    }
    stones.push_back(stone);
  }
  return stones;
}()) {}


std::vector<std::string> SimpleProblem::get_field_str() const
{
  return field;
}

std::vector<std::vector<std::string>> SimpleProblem::get_stones_str() const
{
  return stones;
}

std::vector<std::string> SimpleProblem::get_stone_str(int idx) const
{
  return stones[ idx ];
}

int SimpleProblem::num_of_stones() const
{
  return num;
}

