#include "ExtendedStone.hpp"
#include "ExtendedField.hpp"
#include <bitset>

ExtendedStone::ExtendedStone(std::vector<std::string> strs, int index) {
	ID = index;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			mat[0][0][i][j] = strs[i][j] == '1';
			mat[0][1][i][7 - j] = mat[0][0][i][j];
			if (strs[i][j] == '1')zk++;
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

	//
	id = index;
	//for (int r = 0; r < 2; ++r) {
	//	for (int a = 0; a < 4; ++a) {
	//		for (int i = 0; i < 8; ++i) {
	//			for (int j = 0; j < 8; ++j) {
	//				std::cout << mat[a][r][i][j] ? "1" : "0";
	//			}
	//			std::cout << std::endl;
	//		}
	//		std::cout << std::endl;
	//	}
	//	std::cout << std::endl;
	//}
	for (int r = 0; r < 2; r++) {
		for (int a = 0; a < 4; a++) {
			//マージンを計算
			int x_min = 8;
			int x_max = -1;
			int y_min = 8;
			int y_max = -1;

			for (int y = 0; y < 8; y++) {
				for (int x = 0; x < 8; x++) {
					if (mat[a][r][y][x]) {
						if (x < x_min)x_min = x;
						if (x > x_max)x_max = x;
						if (y < y_min)y_min = y;
						if (y > y_max)y_max = y;
					}
				}
			}
			margin[r][a][LEFT]  = - x_min;
			margin[r][a][RIGHT] = 31 - x_max;
			margin[r][a][UP]    = - y_min;
			margin[r][a][DOWN]  = 31 - y_max;

			//x_min, y_minほどずらした位置からビットを立てる
			for (int y = y_min; y < 8; y++) {
				for (int x = x_min; x < 8; x++) {
					if (mat[a][r][y][x]) {
						set_bit(r, a, x - x_min, y - y_min);
					}
				}
			}

			for (int y = 0; y < 10; y++) {
				for (int x = 0; x < 10; x++) {
					if (x > 0 && y > 0 && is_set_bit(r, a, x - 1, y - 1))continue;
					//上下左右で隣接するマスにブロックが存在するかどうか調べる
					for (int t = 0; t < 4; t++) {
						int x_tmp = x + ExtendedField::tx[t] - 1;
						int y_tmp = y + ExtendedField::ty[t] - 1;
						if (x_tmp < 0 || x_tmp >= 8 || y_tmp < 0 || y_tmp >= 8)continue;
						if (is_set_bit(r, a, x_tmp, y_tmp)) {
							set_neighbor(r, a, x , y);
						}
					}
				}
			}
			//std::cout << "stone No. : " << index  << " " << (r==0?"H ":"T ") << a * 90 << std::endl;
			//std::cout << "bit:\n" << get_bit_str(r, a) << std::endl;
			//std::cout << "neighbor:\n" << get_neighbor_str(r, a) << std::endl;
		}
	}


}

bool ExtendedStone::at(int x, int y, int reverse, int angle) const {
	return mat[angle][reverse][y][x];
}

RowBit* ExtendedStone::get_bit(int rev, int rot) {
	return bitmat[rev][rot];
}
RowBit* ExtendedStone::get_neighbor(int rev, int rot) {
	return around_bit[rev][rot];
}

void ExtendedStone::set_bit(int r, int a, int x, int y) {
	RowBit tmp = 1 << 31;
	tmp >>= x;
	bitmat[r][a][y] |= tmp;
}

void ExtendedStone::set_neighbor(int r, int a, int x, int y) {
	RowBit tmp = 1 << 31;
	tmp >>= x;
	around_bit[r][a][y] |= tmp;
}


int ExtendedStone::getZK()
{
	return zk;
}

int ExtendedStone::getID() {
	return id;
}

std::pair<double, double> ExtendedStone::center(int reverse, int angle) const
{
	return std::pair<double, double>();
}

std::string ExtendedStone::get_bit_str(int r, int a) {
	std::string str = "";
	for (int i = 0; i < 32; i++) {
		std::bitset<32> bits(bitmat[r][a][i]);
		str += bits.to_string();
		if (i < 31)str += "\n";
	}
	return str;
}

std::string ExtendedStone::get_neighbor_str(int r, int a) {
	std::string str = "";
	for (int i = 0; i < 32; i++) {
		std::bitset<32> bits(around_bit[r][a][i]);
		str += bits.to_string();
		if (i < 31)str += "\n";
	}
	return str;
}
bool ExtendedStone::is_set_bit(int r, int a, int x, int y) {
	return ((bitmat[r][a][y] >> (31 - x)) & 1) > 0;
}
bool ExtendedStone::is_set_neighbor(int r, int a, int x, int y) {
	return ((around_bit[r][a][y] >> (31 - x)) & 1) > 0;
}
//x, y:-7..31
bool ExtendedStone::movable(int r, int a, int x, int y) {
	return margin[r][a][LEFT] <= x && margin[r][a][RIGHT] >= x
		&& margin[r][a][UP] <= y && margin[r][a][DOWN] >= y;
}
RowBit ExtendedStone::get_bit_row(int r, int a, int x, int y) {
	return bitmat[r][a][y] >> (x - margin[r][a][LEFT]);
}
RowBit ExtendedStone::get_neighbor_row(int r, int a, int x, int y) {
	int xm = x - margin[r][a][LEFT] - 1;
	if (xm < 0) return around_bit[r][a][y + 1] << (-xm);
	else return around_bit[r][a][y + 1] >> xm;
}
int ExtendedStone::get_y_margin(int r, int a)
{
	return margin[r][a][UP];
}