#pragma once

#include <string>
#include <iostream>
#include "boost/format.hpp"
class PlaceInfo {
public:
	PlaceInfo();
	PlaceInfo(int stone_id, int x, int y, int reverse, int angle);
	int get_id();
	int get_x();
	int get_y();
	int get_angle();
	int get_reverse();
	std::string get_str();
private:
	unsigned int value;
};