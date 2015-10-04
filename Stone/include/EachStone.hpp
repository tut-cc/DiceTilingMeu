#pragma once

#include <utility>
#include <string>
#include <bitset>
#include "Constants.hpp"

class EachStone {
private:
	RowBit bitmat[32] = {};
	RowBit around[32] = {};
	int margin[4];
	int ra_id;

	std::pair<int, int> place;
public:

	EachStone() = default;
	EachStone(int r, int a, bool mat[8][8]);

	virtual void set_bit(int x, int y);
	virtual void set_neighbor(int x, int y);

	bool is_set_bit(int x, int y);
	bool is_set_neighbor(int x, int y);

	virtual RowBit* get_bit();
	virtual RowBit* get_neighbor();

	std::string get_bit_str();
	std::string get_neighbor_str();

	RowBit get_bit_row(int x, int y);
	RowBit get_neighbor_row(int x, int y);

	bool movable(int x, int y);

	int get_y_margin();

	std::pair<int, int> get_place();

	bool operator ==(const EachStone& other) const {
		for (int i = 0; i < 32; ++i) {
			if (bitmat[i] != other.bitmat[i]) return false;
		}
		return true;
	}
	bool operator !=(const EachStone& other) const {
		for (int i = 0; i < 32; ++i) {
			if (bitmat[i] != other.bitmat[i]) return true;
		}
		return false;
	}
	size_t getHashCode() const {
		size_t h = 0;
		for (int i = 0; i < 32; i++) {
			h ^=std::hash<int>()(bitmat[i]);
		}
		return h;
	}

	bool equals(const EachStone& other) const {
		for (int i = 0; i < 32; ++i) {
			if (bitmat[i] != other.bitmat[i]) return false;
		}
		return true;
	}
};
namespace std
{
	template<> struct hash<EachStone>
	{
		std::size_t operator()(const EachStone& key) const
		{
			return key.getHashCode();
		}
	};
	template<> struct equal_to<EachStone>
	{
		bool operator ()(const EachStone& a, const EachStone& b) const
		{
			return a.equals(b);
		}
	};
}
