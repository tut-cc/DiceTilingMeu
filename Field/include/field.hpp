//石を置く盤面のクラス
#pragma once
#include "stone.hpp"
#include <memory>
#include <tuple>
#include <iostream>
#include <vector>

class Field {
  private:
    std::vector<std::tuple<std::weak_ptr<Stone>, int, int, int, int>> history;
  public:
    virtual ~Field() = default;
    virtual bool at(int x, int y) const = 0;
    virtual bool appliable(std::weak_ptr<Stone> s, int x, int y, int reverse, int angle) const = 0;
    virtual void apply(std::weak_ptr<Stone> s, int x, int y, int reverse, int angle) = 0;
    virtual std::unique_ptr<Field> clone() const = 0;
    virtual int score() const;
    virtual decltype(history) get_history() const final;
    virtual std::ostream& operator<<(std::ostream& os) const final;
};

