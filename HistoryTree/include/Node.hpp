#pragma once

#include "PlaceInfo.hpp"

class Node {
public:
	int parent_index;
	PlaceInfo info;
	Node(int parent_index, PlaceInfo info);
};