#include "SimpleField.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

SimpleField::SimpleField(std::vector<std::string> strs)
{
  for (int i = 0; i < 32; ++i) {
    for(int j = 0; j < 32; ++j) {
      mat[i][j] = strs[i][j] == '1';
    }
  }
}

SimpleField::SimpleField(const bool mat[32][32])
{
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      this -> mat[i][j] = mat[i][j];
    }
  }
}

bool SimpleField::at(int x, int y) const
{
  return mat[y][x];
}

bool SimpleField::appliable(std::weak_ptr<Stone> s, int x, int y, int rotate) const
{
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if ( mat[y + i][x + j] & s.lock() -> at(j, i, rotate) ) {
        return false;
      }
    }
  }
  return true;
}

void SimpleField::apply(std::weak_ptr<Stone> s, int x, int y, int rotate)
{
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
#ifdef _DEBUG
      if ( mat[y + i][x + j] & s.lock() -> at(j, i, rotate) ) {
        throw std::runtime_error("cannot apply");
      }
#endif
      mat[y + i][x + j] |= s.lock() -> at(j, i, rotate);
    }
  }
}

std::unique_ptr<Field> SimpleField::clone() const
{
  return std::move(std::unique_ptr<Field>(new SimpleField(mat)));
}

