//石を置く盤面のクラス
#pragma once
#include "stone.hpp"
#include <memory>
#include <tuple>
#include <iostream>
#include <vector>

class Field {
  protected:
    std::vector<std::tuple<std::shared_ptr<Stone>, int, int, int, int>> history;
  public:
    Field() = default;
    Field(const decltype(history) &);
    Field(const Field &) = default;
    Field(Field &&) = default;
    virtual ~Field() = default;
    virtual bool at(int x, int y) const = 0;
    virtual bool appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const = 0;
    virtual void apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) = 0;
    virtual std::unique_ptr<Field> clone() const = 0;
    virtual int score() const;
    virtual decltype(history) get_history() const final;
    friend std::ostream& operator<<(std::ostream& os, const Field& f);
};

