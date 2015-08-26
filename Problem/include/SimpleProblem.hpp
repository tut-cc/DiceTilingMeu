#pragma once

#include "Problem.h"

class SimpleProblem : public Problem {
  private:
    std::vector<std::string> field;
    int num;
    std::vector<std::vector<std::string>> stones;

  public:
    SimpleProblem();
    virtual ~SimpleProblem();
    virtual std::vector<std::string> get_field_str() const;
    virtual std::vector<std::vector<std::string>> get_stones_str() const;
    virtual std::vector<std::string> get_stone_str(int idx) const;
    virtual int num_of_stones() const;
};

