#include "field.h"

void Field::apply(std::weak_ptr<Stone> s, int x, int y, int reverse, int angle)
{
  history.push_back(std::make_tuple(s, x, y, reverse, angle));
}

decltype(((Field *)nullptr)->get_history()) Field::get_history() const
{
  return history;
}

int Field::score() const
{
  int cnt = 0;

  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      cnt += !at(j, i);
    }
  }

  return cnt;
}

std::ostream& operator<<(std::ostream& os, const Field& f)
{
  int index = 0;
  for ( unsigned int i = 0; i < f.history.size(); ++i ) {
    for ( ; index < std::get<0>(f.history[i]).lock() -> identify(); ++index ) {
      os << std::endl;
    }
    int x, y, rev, ang;
    std::tie(std::ignore, x, y, rev, ang) = f.history[i];
    os << x << " " << y << " " << (rev ? "T" : "H") << " " << (ang * 90) << std::endl;
    ++index;
  }
  return os;
}

