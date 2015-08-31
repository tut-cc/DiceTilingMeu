#include "field.hpp"

void Field::apply(std::weak_ptr<Stone> s, int x, int y, int reverse, int angle)
{
  history.push_back(std::make_tuple(s, x, y, reverse, angle));
}

decltype(((Field *)nullptr)->get_history()) Field::get_history() const
{
  return history;
}

std::ostream& Field::operator<<(std::ostream& os) const
{
  int index = 0;
  for ( unsigned int i = 0; i < history.size(); ++i ) {
    for ( ; index < std::get<0>(history[i]).lock() -> identify(); ++index ) {
      os << std::endl;
    }
    int x, y, rev, ang;
    std::tie(std::ignore, x, y, rev, ang) = history[i];
    os << x << " " << y << " " << (rev ? "T" : "H") << " " << (ang * 90) << std::endl;
    ++index;
  }
  return os;
}

