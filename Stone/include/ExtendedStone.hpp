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

	void array_to_bit(int rev, int rot);
public:
	ExtendedStone(std::vector<std::string> strs, int ID);
	virtual ~ExtendedStone() = default;
	virtual bool at(int x, int y, int reverse, int angle) const override;
	virtual int getID();

	//— •Ô‚µA‰ñ“]Œã‚Ì‚È‚ñ‚©‚ğ•Ô‚·
	virtual __int32* get_bit(int rev, int rot);
	virtual __int32* get_neighbor(int rev, int rot);
	virtual void set_bit(int rev, int rot, int x, int y);

	//zk”‚ğ•Ô‚·
	virtual int getZK();
};

