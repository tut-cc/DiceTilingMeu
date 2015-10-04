#include "EachStone.hpp"

EachStone::EachStone(int r, int a, bool mat[8][8])
{
	place.first = r;
	place.second = a;

	//マージンを計算
	int x_min = 8;
	int x_max = -1;
	int y_min = 8;
	int y_max = -1;

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (mat[y][x]) {
				if (x < x_min)x_min = x;
				if (x > x_max)x_max = x;
				if (y < y_min)y_min = y;
				if (y > y_max)y_max = y;
			}
		}
	}
	margin[LEFT] = -x_min;
	margin[RIGHT] = 31 - x_max;
	margin[UP] = -y_min;
	margin[DOWN] = 31 - y_max;

	//x_min, y_minほどずらした位置からビットを立てる
	for (int y = y_min; y < 8; y++) {
		for (int x = x_min; x < 8; x++) {
			if (mat[y][x]) {
				set_bit(x - x_min, y - y_min);
			}
		}
	}

	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			if (x > 0 && y > 0 && is_set_bit(x - 1, y - 1))continue;
			//上下左右で隣接するマスにブロックが存在するかどうか調べる
			for (int t = 0; t < 4; t++) {
				int x_tmp = x + tx[t] - 1;
				int y_tmp = y + ty[t] - 1;
				if (x_tmp < 0 || x_tmp >= 8 || y_tmp < 0 || y_tmp >= 8)continue;
				if (is_set_bit(x_tmp, y_tmp)) {
					set_neighbor(x, y);
				}
			}
		}
	}

//	std::cout << r << " " << a << "\n" << get_bit_str() << std::endl;
}

bool EachStone::is_set_bit(int x, int y) {
	return ((bitmat[y] >> (31 - x)) & 1) > 0;
}
bool EachStone::is_set_neighbor(int x, int y) {
	return ((around[y] >> (31 - x)) & 1) > 0;
}

RowBit * EachStone::get_bit() { return bitmat; }
RowBit * EachStone::get_neighbor() { return around; }

std::string EachStone::get_bit_str()
{
	std::string str = "";
	for (int i = 0; i < 32; i++) {
		std::bitset<32> bits(bitmat[i]);
		str += bits.to_string();
		if (i < 31)str += "\n";
	}
	return str;
}

std::string EachStone::get_neighbor_str()
{
	std::string str = "";
	for (int i = 0; i < 32; i++) {
		std::bitset<32> bits(around[i]);
		str += bits.to_string();
		if (i < 31)str += "\n";
	}
	return str;
}

RowBit EachStone::get_bit_row(int x, int y)
{
	return bitmat[y] >> (x - margin[LEFT]);
}

RowBit EachStone::get_neighbor_row(int x, int y)
{
	int xm = x - margin[LEFT] - 1;
	if (xm < 0) return around[y + 1] << (-xm);
	else return around[y + 1] >> xm;
}

bool EachStone::movable(int x, int y)
{
	return margin[LEFT] <= x && margin[RIGHT] >= x && margin[UP] <= y && margin[DOWN] >= y;
}

int EachStone::get_y_margin()
{
	return margin[UP];
}

std::pair<int, int> EachStone::get_place()
{
	return place;
}

void EachStone::set_bit(int x, int y) {
	RowBit tmp = 1 << 31;
	tmp >>= x;
	bitmat[y] |= tmp;
}

void EachStone::set_neighbor(int x, int y) {
	RowBit tmp = 1 << 31;
	tmp >>= x;
	around[y] |= tmp;
}