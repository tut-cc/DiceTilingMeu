#include "ExtendedField.hpp"
#include <stdexcept>
#include <iostream>
#include <string>
#include <bitset>
#include <intrin.h>


int ExtendedField::space_count = 0;

ExtendedField::ExtendedField(std::vector<std::string> strs) {
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			if (strs[i][j] == '1') {
				set_bit(j, i);
			}
			else {
				space_count++;
			}
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
	//for (int i = 0; i < 32; ++i) {
	//	for (int j = 0; j < 32; ++j) {
	//		this->mat[i][j] = f.at(j, i);
	//		ok[i][j] = f.at_ok(j, i);
	//	}
	//}
	for (int i = 0; i < 32; i++) {
		bitmat[i] = f.bitmat[i];
		next_block[i] = f.next_block[i];
	}
	block_count = f.block_count;
	value = f.value;
	parent_idx = f.parent_idx;
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
//	Field::apply(s, x, y, reverse, angle);
#ifdef _DEBUG
	if (!appliable_bit(s, x, y, reverse, angle)) {

		throw std::runtime_error("cannot apply!!! ");
	}
#endif
	int y_margin = s->get_y_margin(reverse, angle);
	for (int i = 0; i < 8; i++) {
		if ((i + y - y_margin) < 0 || (i + y - y_margin) > 31)continue;
		bitmat[i + y - y_margin] |= s->get_bit_row(reverse, angle, x, i);
	}

	for (int i = -1; i < 9; i++) {
		int ym = i + y - y_margin;
		if (ym < 0 || ym > 31)continue;
		next_block[ym] = (next_block[ym] | s->get_neighbor_row(reverse, angle, x, i)) & (~bitmat[ym]);
	}
	block_count += s->getZK();
	value = -1;

	//ヒストリー追加
	PlaceInfo p = PlaceInfo(s->identify(), x, y, reverse, angle);
	parent_idx = HistoryTree::add(parent_idx, p);
}
bool ExtendedField::appliable_bit(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) const {
	if (!s->movable(reverse, angle, x, y))return false;
	int y_margin;
	y_margin = s->get_y_margin(reverse, angle);
	//置けるかどうか
	for (int i = 0; i < 8; i++) {
		if ((i + y - y_margin) < 0 || (i + y - y_margin) > 31)continue;
		if ((bitmat[i + y - y_margin] & s->get_bit_row(reverse, angle, x, i)) != 0)return false;
	}
	//すでに配置した石と隣り合っているかどうか
	bool n_flag = block_count == 0;
	for (int i = 0; i < 32; i++) {
		if ((i + y - y_margin) < 0 || (i + y - y_margin) > 31)continue;
		if ((next_block[i + y - y_margin] & s->get_bit_row(reverse, angle, x, i)) != 0)return true;
	}

	if (n_flag)return true;

	return false;
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
		//for (int i = 0; i < 31; i++) count += __popcnt(bitmat[i] ^ bitmat[i + 1]);
		for (int i = 1; i < 32; i++) count += __popcnt(bitmat[i - 1] ^ bitmat[i]);
		for (int i = 0; i < 32; i++) {
			//count += __popcnt(bitmat[i] & (bitmat[i] >> 1));
			//count += __popcnt(bitmat[i] & (bitmat[i] << 1));
			count += __popcnt(this->next_block[i]);
		}
		value = -count;
		//std::cout << value << std::endl;
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

std::string ExtendedField::get_bit_str() {
	std::string str = "";
	for (int i = 0; i < 32; i++) {
		std::bitset<32> bits(bitmat[i]);
		str += bits.to_string();
		if (i < 31)str += "\n";
	}
	return str;
}
std::string ExtendedField::get_neighbor_str() {
	std::string str = "";
	for (int i = 0; i < 32; i++) {
		std::bitset<32> bits(next_block[i]);
		str += bits.to_string();
		if (i < 31)str += "\n";
	}
	return str;
}

void ExtendedField::set_bit(int x, int y) {
	RowBit tmp = 1 << 31;
	tmp >>= x;
	bitmat[y] |= tmp;
}

int ExtendedField::get_bit(int x, int y) {
	return (bitmat[y] >> (31 - x)) & 1;
}
