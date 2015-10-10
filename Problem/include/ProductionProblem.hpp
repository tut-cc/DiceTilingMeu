#pragma once

#include "problem.hpp"

class ProductionProblem : public Problem {
private:
  std::vector<std::string> field;
  int num;
  std::vector<std::vector<std::string>> stones;

public:
  ProductionProblem(const std::string &);
  virtual ~ProductionProblem() = default;
  virtual std::vector<std::string> get_field_str() const override;
  virtual std::vector<std::vector<std::string>> get_stones_str() const override;
  virtual std::vector<std::string> get_stone_str(int idx) const override;
  virtual int num_of_stones() const override;
};

