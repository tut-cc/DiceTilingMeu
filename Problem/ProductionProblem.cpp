#include "ProductionProblem.hpp"
#include <string>
#include <sstream>
#include <iostream>

ProductionProblem::ProductionProblem(const std::string & raw)
{
  std::stringstream ss(raw);

  field = [&]() {
    std::vector<std::string> list;
    for (int i = 0; i < 32; ++i) {
      std::string str;
      ss >> str;
      list.push_back(str);
    }
    return list;
  }();
  stones = [&]() {
    std::vector<std::vector<std::string>> stones;
    ss >> num;

    for (int i = 0; i < num; ++i) {
      std::vector<std::string> stone;
      for (int i = 0; i < 8; ++i) {
        std::string str;
        ss >> str;
        stone.push_back(str);
      }
      stones.push_back(stone);
    }
    return stones;
  }();
}


std::vector<std::string> ProductionProblem::get_field_str() const
{
  return field;
}

std::vector<std::vector<std::string>> ProductionProblem::get_stones_str() const
{
  return stones;
}

std::vector<std::string> ProductionProblem::get_stone_str(int idx) const
{
  return stones[idx];
}

int ProductionProblem::num_of_stones() const
{
  return num;
}

