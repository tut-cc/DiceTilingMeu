#pragma once

#include "Node.hpp"
#include "PlaceInfo.hpp"
#include <vector>
#include <algorithm>

class HistoryTree {
public:
	static int init_history_tree(int stone_num);
	static int add(int parent_idx, PlaceInfo& info);
	static std::vector<Node> tree;

	static std::string get_answer(int index);
	static void clear();
private:
	HistoryTree();
	static int stone_num;
};