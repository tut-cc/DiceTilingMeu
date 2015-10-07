#include "ExtendedStone.hpp"
#include "ExtendedField.hpp"
#include <bitset>

std::vector<std::pair<int, int>> ExtendedStone::get_place_list()
{
	return place_list;
}

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

	std::unordered_set<EachStone> tmp_sts;
	for (int r = 0; r < 2; r++) {
		for (int a = 0; a < 4; a++) {
			EachStone es(r, a, mat[a][r]);
			stones[r][a] = es;
			tmp_sts.insert(es);
			//std::cout << "stone No. : " << index  << " " << (r==0?"H ":"T ") << a * 90 << std::endl;
			//std::cout << "bit:\n" << get_bit_str(r, a) << std::endl;
			//std::cout << "neighbor:\n" << get_neighbor_str(r, a) << std::endl;
		}
	}
	for (auto st : tmp_sts) {
		place_list.push_back(st.get_place());
	}
}

bool ExtendedStone::at(int x, int y, int reverse, int angle) const {
	return mat[angle][reverse][y][x];
}

RowBit* ExtendedStone::get_bit(int r, int a) {
	return stones[r][a].get_bit();
}
RowBit* ExtendedStone::get_neighbor(int r, int a) {
	return stones[r][a].get_neighbor();
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
	return stones[r][a].get_bit_str();
}

std::string ExtendedStone::get_neighbor_str(int r, int a) {
	return stones[r][a].get_neighbor_str();
}
bool ExtendedStone::is_set_bit(int r, int a, int x, int y) {
	return stones[r][a].is_set_bit(x, y);
}
bool ExtendedStone::is_set_neighbor(int r, int a, int x, int y) {
	return stones[r][a].is_set_neighbor(x, y);
}
//x, y:-7..31
bool ExtendedStone::movable(int r, int a, int x, int y) {
	return stones[r][a].movable(x, y);
}
RowBit ExtendedStone::get_bit_row(int r, int a, int x, int y) {
	return stones[r][a].get_bit_row(x, y);
}
RowBit ExtendedStone::get_neighbor_row(int r, int a, int x, int y) {
	return stones[r][a].get_neighbor_row(x, y);
}
int ExtendedStone::get_y_margin(int r, int a)
{
	return stones[r][a].get_y_margin();
}