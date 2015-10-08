#include "HistoryTree.hpp"

/*
�e�X���b�h�ɑ΂��āANode�̔z������(vector�Ŏ���)
�|�C���^�ł͂Ȃ�int�^�ŃC���f�b�N�X��ێ�����
Node�́A�e�̃m�[�h�̃C���f�b�N�X�������Ă���t�ɂ��ǂ邱�ƂŎ菇�𕜌��ł���
add��static�ɂȂ��Ă��āA�V�����C���f�b�N�X��Ԃ�
	�Evector��push_back

HistoryTree.add(th_id, par_id, new_node)�݂����Ȋ�����add
*/

std::vector<Node> HistoryTree::tree;
int HistoryTree::stone_num;

HistoryTree::HistoryTree(){}

int HistoryTree::init_history_tree(int stone_num)
{
	HistoryTree::stone_num = stone_num;
	Node n(-1, PlaceInfo());
	n.own_index = 0;
	HistoryTree::tree.push_back(n);
	return 0;
}
int HistoryTree::add(int parent_idx, PlaceInfo & info)
{
	int ret = 0;
#pragma omp critical
	{
		ret = tree.size();
		Node n(parent_idx, info);
		n.own_index = ret;
		tree.push_back(n);
		tree[ret].child_count++;
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
	auto tmp = std::move(tree[0]);
	tree.clear();
	tree.push_back(tmp);
}

void HistoryTree::remove(int index)
{
	auto rm = std::move(tree[index]);
	tree[rm.parent_index].child_count--;
	while (rm.parent_index != 0 && tree[rm.parent_index].child_count == 0) {
		auto tmp = std::move(tree[rm.parent_index]);
		tree[tmp.parent_index].child_count--;
		rm = std::move(tmp);
	}
}

