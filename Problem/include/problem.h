#pragma once

#include <vector>
#include <string>

class Problem {
  public:
    virtual ~Problem() = default;
    virtual std::vector<std::string> get_field_str() const = 0;
    virtual std::vector<std::vector<std::string>> get_stones_str() const = 0;
    virtual std::vector<std::string> get_stone_str(int idx) const = 0;
    virtual int num_of_stones() const = 0;
};

