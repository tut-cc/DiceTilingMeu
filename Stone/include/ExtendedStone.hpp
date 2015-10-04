#pragma once
#include "stone.hpp"
#include "EachStone.hpp"
#include "Constants.hpp"
#include <vector>
#include <string>
#include <unordered_set>

class ExtendedStone : public Stone {
private:
	bool mat[4][2][8][8];

	int id;
	int zk = 0;

	EachStone stones[2][4];
	std::vector<std::pair<int, int>> place_list;

public:
	std::vector<std::pair<int, int>> get_place_list();
	ExtendedStone(std::vector<std::string> strs, int index);
	virtual ~ExtendedStone() = default;
	virtual bool at(int x, int y, int reverse, int angle) const override;
	virtual int getID();
	virtual std::pair<double, double> center(int reverse, int angle) const override;

	//裏返し、回転後のなんかを返す
	virtual RowBit* get_bit(int r, int a);
	virtual RowBit* get_neighbor(int r, int a);

	std::string get_bit_str(int r, int a);
	std::string get_neighbor_str(int r, int a);

	RowBit get_bit_row(int r, int a, int x, int y);
	RowBit get_neighbor_row(int r, int a, int x, int y);

	//そこのビットが立っているか確かめる
	bool is_set_bit(int r, int a, int x, int y);
	bool is_set_neighbor(int r, int a, int x, int y);

	//その座標まで移動できるかを返す
	bool movable(int r, int a, int x, int y);
	//zk数を返す
	virtual int getZK();

	int get_y_margin(int r, int a);
};

