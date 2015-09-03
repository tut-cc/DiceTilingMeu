#pragma once
#include "stone.hpp"
#include <vector>
#include <string>

class ExtendedStone : public Stone {
private:
	bool mat[4][2][8][8];
	__int32 bitmat[2][4][32];
	__int32 around_bit[2][4][32];
	int margin[2][4][4];

	int id;
	int zk;
public:
	ExtendedStone(std::vector<std::string>, int ID);
	virtual ~ExtendedStone() = default;
	virtual bool at(int x, int y, int reverse, int angle) const override;
	virtual int getID();

	//���Ԃ��A��]��̂Ȃ񂩂�Ԃ�
	virtual __int32* get_bit(int rev, int rot);
	virtual __int32* get_neighbor(int rev, int rot);

	//zk����Ԃ�
	virtual int getZK();
};

