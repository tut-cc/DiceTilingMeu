#include "BoostBitStone.hpp"

BoostBitStone::BoostBitStone(const std::vector<std::string> & strs, int index)
{
  bool mat[4][2][32][32];
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

  for (int a = 0; a < 4; ++a) {
    for (int r = 0; r < 2; ++r) {
      boost::multiprecision::uint1024_t k = 1;
      boost::multiprecision::uint1024_t m = 0;
      for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
          if (mat[a][r][i][j]) {
            m |= k;
          }
          k <<= 1;
        }
      }
      this->mat[a][r] = m;
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

const boost::multiprecision::uint1024_t & BoostBitStone::raw(int reverse, int angle) const
{
  // TODO: insert return statement here
}

bool BoostBitStone::at(int x, int y, int reverse, int angle) const
{
  return false;
}

std::pair<double, double> BoostBitStone::center(int reverse, int angle) const
{
  return std::pair<double, double>();
}
