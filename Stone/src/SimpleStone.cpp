#include "SimpleStone.hpp"
#include <string>
#include <iostream>

SimpleStone::SimpleStone()
{
  for (int i = 0; i < 8; ++i) {
    std::string str;
    std::cin >> str;
    for (int j = 0; j < 8; ++j) {
      mat[i][j] = str.at(j);
    }
  }
  angle = 0;
}

void SimpleStone::rotate()
{
  angle = (angle + 1) % 4;
  bool nxt[32][32];
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      nxt[i][j] = this -> mat[7 - j][i];
    }
  }
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      this -> mat[i][j] = nxt[i][j];
    }
  }
}

bool SimpleStone::at(int x, int y, int angle)
{
  for( int i = this->angle; i != angle; this -> rotate() );
  return mat[y][x];
}

