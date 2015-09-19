#include "SimpleStone.hpp"
#include <string>
#include <iostream>

SimpleStone::SimpleStone(std::vector<std::string> strs, int index)
{
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      mat[0][0][i][j] = strs[i][j] == '1';
      mat[0][1][i][7 - j] = mat[0][0][i][j];
    }
  }
  for (int a = 1; a < 4; ++a) {
    for (int r = 0; r < 2; ++r) {
      for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
          mat[a][r][i][j] = mat[a - 1][r][7 - j][i];
        }
      }
    }
  }

  int cnt = 0;
  double sx = 0;
  double sy = 0;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (mat[0][0][i][j]) {
        ++cnt;
        sx += j;
        sy += i;
      }
    }
  }
  sx /= (double)cnt;
  sy /= (double)cnt;
  centers[0][0] = std::make_pair(sx, sy);
  centers[0][1] = std::make_pair(7.0 - sx, sy);
  for (int a = 1; a < 4; ++a) {
    for (int r = 0; r < 2; ++r) {
      centers[a][r] = std::make_pair(7.0 - centers[a - 1][r].first, centers[a - 1][r].second);
    }
  }

  ID = index;
}

bool SimpleStone::at(int x, int y, int reverse, int angle) const
{
  return mat[angle][reverse][y][x];
}

std::pair<double, double> SimpleStone::center(int reverse, int angle) const
{
  return centers[angle][reverse];
}
