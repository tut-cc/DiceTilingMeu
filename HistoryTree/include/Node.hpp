#pragma once

#include "PlaceInfo.hpp"

class Node {
public:
	int parent_index;
	int own_index;
	int child_count = 0;
	PlaceInfo info;
	Node(int parent_index, PlaceInfo info);
};