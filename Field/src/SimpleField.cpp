#include "SimpleField.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

SimpleField::SimpleField(std::vector<std::string> strs)
{
  for (int i = 0; i < 32; ++i) {
    for(int j = 0; j < 32; ++j) {
      mat[i][j] = strs[i][j] == '1';
      ok[i][j] = true;
    }
  }
}

SimpleField::SimpleField(const bool mat[32][32], const decltype(((Field *)nullptr)->get_history()) & src = {}) : Field(src)
{
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      this -> mat[i][j] = mat[i][j];
      ok[i][j] = true;
    }
  }
}

bool SimpleField::at(int x, int y) const
{
  return mat[y][x];
}

bool SimpleField::appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const
{
  bool adjf = false;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (y + i < 0 || 32 <= y + i || x + j < 0 || 32 <= x + j) {
        if( s -> at(j, i, reverse, angle) ) {
          return false;
        }
        continue;
      }
      if ( mat[y + i][x + j] && s -> at(j, i, reverse, angle) ) {
        return false;
      }
      adjf |= ok[y + i][x + j];
    }
  }
  return adjf;
}

void SimpleField::apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle)
{
  Field::apply(s, x, y, reverse, angle);
#ifdef _DEBUG
  if(!appliable(s, x, y, reverse, angle)) {
    throw std::runtime_error("cannot apply");
  }
#endif
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      ok[i][j] = 0;
    }
  }
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if ( x + j < 0 && 32 <= x + j && y + i < 0 && 32 <= y + i) continue;
      mat[y + i][x + j] |= s -> at(j, i, reverse, angle);
      for (int k = 0; k < 4; ++k) {
        static const int ofs[4][2] = {
          {0, 1},
          {1, 0},
          {-1, 0},
          {0, -1}
        };
        int nx = x + j + ofs[k][0];
        int ny = y + i + ofs[k][1];
        if (nx < 0 || 32 <= nx || ny < 0 || 32 <= ny) {
          continue;
        }
        ok[ny][nx] |= true;
      }
    }
  }

  value = -1;
}

std::unique_ptr<Field> SimpleField::clone() const
{
  auto ptr = std::unique_ptr<Field>(new SimpleField(mat, history));
  return std::move(ptr);
}

const int SimpleField::tx[] = { 1, 0, -1, 0 };
const int SimpleField::ty[] = { 0, -1, 0, 1 };

int SimpleField::h(){
	if (value == -1){
		int count = 0;
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (mat[i][j] == 0)continue;
				bool f = false;
				for (int t = 0; t < 4; t++){
					int nx = j + tx[t];
					int ny = i + ty[t];
					if (nx < 0 || 32 <= nx || ny < 0 || 32 <= ny) {
						continue;
					}
					if (mat[nx][ny] == 0){
						f = true;
					}
				}
				if (f)count++;
			}
		}
		value = count;
	}
	return value;
}
