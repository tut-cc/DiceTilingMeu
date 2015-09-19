#pragma once
#include "stone.hpp"
#include <vector>
#include <string>

typedef unsigned __int32 RowBit;

class ExtendedStone : public Stone {
private:
	bool mat[4][2][8][8];
	RowBit bitmat[2][4][32] = {};
	RowBit around_bit[2][4][32] = {};
	int margin[2][4][4];

	int id;
	int zk = 0;

	static const int RIGHT = 0;
	static const int UP = 1;
	static const int LEFT = 2;
	static const int DOWN = 3;
public:
	ExtendedStone(std::vector<std::string> strs, int index);
	virtual ~ExtendedStone() = default;
	virtual bool at(int x, int y, int reverse, int angle) const override;
	virtual int getID();

	//裏返し、回転後のなんかを返す
	virtual RowBit* get_bit(int r, int a);
	virtual RowBit* get_neighbor(int r, int a);

	virtual void set_bit(int r, int a, int x, int y);
	virtual void set_neighbor(int r, int a, int x, int y);

	std::string get_bit_str(int r, int a);
	std::string get_neighbor_str(int r, int a);

	inline RowBit get_bit_row(int r, int a, int x, int y);
	inline RowBit get_neighbor_row(int r, int a, int x, int y);

	//そこのビットが立っているか確かめる
	bool is_set_bit(int r, int a, int x, int y);
	bool is_set_neighbor(int r, int a, int x, int y);

	//その座標まで移動できるかを返す
	bool movable(int r, int a, int x, int y);
	//zk数を返す
	virtual int getZK();
//	void array_to_bit(int rev, int rot);
};

