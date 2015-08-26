#include "SimpleStone.hpp"
#include <string>
#include <iostream>

SimpleStone::SimpleStone(std::vector<std::string> strs)
{
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      mat[0][i][j] = strs[i][j] == '1';
    }
  }
  for (int a = 1; a < 4; ++a) {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        mat[a][i][j] = mat[a - 1][7 - j][i];
      }
    }
  }
}

bool SimpleStone::at(int x, int y, int angle) const
{
  return mat[angle][y][x];
}

