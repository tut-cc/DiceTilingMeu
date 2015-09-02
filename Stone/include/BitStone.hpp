#pragma once
#include "stone.hpp"
#include <vector>
#include <string>

class BitStone : public Stone {
private:
	bool mat[4][2][8][8];
public:
	BitStone(std::vector<std::string>, int index);
	virtual ~BitStone() = default;
	virtual bool at(int x, int y, int reverse, int angle) const override;
};

