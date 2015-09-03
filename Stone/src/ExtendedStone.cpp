#include "ExtendedStone.hpp"

ExtendedStone::ExtendedStone(std::vector<std::string> strs, int ID) {
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
	id = ID;
}
void ExtendedStone::array_to_bit(int rev, int rot) {

}

bool ExtendedStone::at(int x, int y, int reverse, int angle) const {
	return mat[reverse][angle][y][x];
}

__int32* ExtendedStone::get_bit(int rev, int rot) {
	return bitmat[rev][rot];
}
__int32* ExtendedStone::get_neighbor(int rev, int rot) {
	return around_bit[rev][rot];
}

void ExtendedStone::set_bit(int rev, int rot, int x, int y) {

}

int ExtendedStone::getZK()
{
	return zk;
}

int ExtendedStone::getID() {
	return id;
}