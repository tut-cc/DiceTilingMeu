#include "HistoryTree.hpp"

/*
各スレッドに対して、Nodeの配列を作る(vectorで実装)
ポインタではなくint型でインデックスを保持する
Nodeは、親のノードのインデックスを持っており逆にたどることで手順を復元できる
addもstaticになっていて、新しいインデックスを返す
	・vectorにpush_back

HistoryTree.add(th_id, par_id, new_node)みたいな感じでadd
*/

std::vector<Node> HistoryTree::tree;
int HistoryTree::stone_num;

HistoryTree::HistoryTree(){}

int HistoryTree::init_history_tree(int stone_num)
{
	HistoryTree::stone_num = stone_num;
	HistoryTree::tree.push_back(Node(-1, PlaceInfo()));
	return 0;
}
int HistoryTree::add(int parent_idx, PlaceInfo & info)
{
	int ret = 0;
#pragma omp critical
	{
		ret = tree.size();
		tree.emplace_back(parent_idx, info);
	}
	return ret;
}

std::string HistoryTree::get_answer(int index)
{
	std::string str;
	std::vector<PlaceInfo> tmp_ans;

	int n_idx = index;
	while (n_idx != 0) {
		tmp_ans.push_back(tree[n_idx].info);
		n_idx = tree[n_idx].parent_index;
	}
	std::reverse(tmp_ans.begin(), tmp_ans.end());

	int b_id = -1, t_id;
	for (int i = 0; i < tmp_ans.size(); i++) {
		t_id = tmp_ans[i].get_id();
		for (int j = b_id + 1; j < t_id; j++) { str.push_back('\n'); }
		str += tmp_ans[i].get_str();
		b_id = t_id;
	}
	for (int j = t_id + 1; j < stone_num; j++) { str.push_back('\n'); }

	return str;
}


void HistoryTree::clear()
{
	tree.clear();
}