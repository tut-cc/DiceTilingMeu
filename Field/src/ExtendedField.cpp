#include "ExtendedField.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

const int ExtendedField::tx[] = { 1, 0, -1, 0 };
const int ExtendedField::ty[] = { 0, -1, 0, 1 };

ExtendedField::ExtendedField(std::vector<std::string> strs) {
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			mat[i][j] = strs[i][j] == '1';
			ok[i][j] = true;
		}
	}
	block_count = 0;
	value = -1;
}
ExtendedField::ExtendedField(const bool mat[32][32], const decltype(((Field *)nullptr)->get_history()) & src = {}) : Field(src)
{
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			this->mat[i][j] = mat[i][j];
			ok[i][j] = true;
		}
	}
	block_count = 0;
	value = -1;
}
ExtendedField::ExtendedField(const ExtendedField &f, const decltype(((Field *)nullptr)->get_history()) & src = {}) : Field(src)
{
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			this->mat[i][j] = f.at(j, i);
			ok[i][j] = f.at_ok(j, i);
		}
	}
	block_count = f.block_count;
	value = -1;
}

//ExtendedField::ExtendedField(const ExtendedField & f)
//{
//	for (int i = 0; i < 32; ++i) {
//		for (int j = 0; j < 32; ++j) {
//			mat[i][j] = f.at(j, i);
//			ok[i][j] = true;
//		}
//	}
//	block_count = f.block_count;
//	value = -1;
//}


bool ExtendedField::at(int x, int y) const{
	return mat[y][x];
}
bool ExtendedField::at_ok(int x, int y) const {
	return ok[y][x];
}

bool ExtendedField::appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const { return false; }
void ExtendedField::apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) {}

bool ExtendedField::appliable_ex(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) const {
	bool adjf = false;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (y + i < 0 || 32 <= y + i || x + j < 0 || 32 <= x + j) {
				if (s->at(j, i, reverse, angle)) {
					return false;
				}
				continue;
			}
			if (mat[y + i][x + j] && s->at(j, i, reverse, angle)) {
				return false;
			}
			adjf |= s->at(j, i, reverse, angle) & ok[y + i][x + j];
		}
	}
	return adjf;
}
void ExtendedField::apply_ex(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) {

	Field::apply(s, x, y, reverse, angle);
#ifdef _DEBUG
	if (!appliable_ex(s, x, y, reverse, angle)) {
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
			if (x + j < 0 || 32 <= x + j || y + i < 0 || 32 <= y + i) continue;
			mat[y + i][x + j] |= s->at(j, i, reverse, angle);
			if (!s->at(j, i, reverse, angle)) continue;
			for (int k = 0; k < 4; ++k) {
				static const int ofs[4][2] = {
					{ 0, 1 },
					{ 1, 0 },
					{ -1, 0 },
					{ 0, -1 }
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
	block_count += s->getZK();
}

void ExtendedField::apply_bit(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) {
#ifdef _DEBUG
	if (appliable_bit(s, x, y, reverse, angle)) {

		throw std::runtime_error("cannot apply!!! ");
	}
#endif
	for (int i = y; i < 32; i++) {
		bitmat[i] &= s->get_bit_row(reverse, angle, x, i);
	}
	for (int i = y; i < 32; i++) {
		bitmat[i] &= s->get_bit_row(reverse, angle, x, i);
		next_block[i] = (next_block[i] | s->get_neighbor_row(reverse, angle, x, i)) & (~bitmat[i]);
	}
}
bool ExtendedField::appliable_bit(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) const {
	if (!s->movable(reverse, angle, x, y))return false;
	//置けるかどうか
	for (int i = y; i < 32; i++) {
		if ((bitmat[i] & s->get_bit_row(reverse, angle, x, i)) != 0)return false;
	}
	//すでに配置した石と隣り合っているかどうか
	bool n_flag = false;
	for (int i = 0; i < 32; i++) {
		if ((next_block[i] & s->get_neighbor_row(reverse, angle, x, i)) != 0)return true;
	}

	if (!n_flag)return false;

	return true;
}


std::unique_ptr<Field> ExtendedField::clone() const {
	auto ptr = std::unique_ptr<Field>(new ExtendedField(mat, history));
	return std::move(ptr);
}
std::shared_ptr<ExtendedField> ExtendedField::clone_ex() const {
	auto ptr = std::shared_ptr<ExtendedField>(new ExtendedField(*this, history));
	return std::move(ptr);
}

int ExtendedField::eval_final_score() {
	return block_count;
}
int ExtendedField::eval_select_score() {
	if (value == -1) {
		int count = 0;
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (mat[i][j] == 0)continue;
				bool f = false;
				for (int t = 0; t < 4; t++) {
					int nx = j + tx[t];
					int ny = i + ty[t];
					if (nx < 0 || 32 <= nx || ny < 0 || 32 <= ny) {
						continue;
					}
					if (mat[nx][ny] == 0) {
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

std::ostream& operator << (std::ostream& os, const std::shared_ptr<ExtendedField>& p) {
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			os << p->at(j, i);
		}
		os << std::endl;
	}
	return os;
}
