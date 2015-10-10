#pragma once

#include "ClusteredBeam.hpp"
#include "PlaceLists.hpp"
#include "StateQueue.hpp"
#include "RandomValue.hpp"
#include "RandomQueue.hpp"
#include <SimpleField.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <fstream>

template <class F, class S>
ClusteredBeam<F, S>::ClusteredBeam(std::shared_ptr<Problem> p)
{
	field = std::move(std::shared_ptr<ExtendedField>(new F(p->get_field_str())));

	int s_idx = 0;
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<ExtendedStone>(new S(s, s_idx)));
		s_idx++;
	}
	stones_num = (int)(stones.size());
	HistoryTree::init_history_tree(stones_num);
	RandomValue::init_random();
}


template <class F, class S>
void ClusteredBeam<F, S>::solve()
{
	std::cout << "field:\n" << field->get_bit_str() << std::endl;
	std::shared_ptr<ExtendedField> result = std::move(field->clone_ex());

	/*
	石をn個ずつのクラスタに分け、各クラスタの石を順番に置いて良いものをキューに入れる
	これを順に行う
	*/

	int max_cluster_num = stones_num / CLUSTER_SIZE;
	int cluster_size2 = stones_num % CLUSTER_SIZE;
	std::vector<std::vector<std::vector<int>>> list1;
	for (int i = 1; i <= CLUSTER_SIZE; i++) {
		list1.push_back(ClusteredBeam::combination(CLUSTER_SIZE, i));
	}

	//キュー
	StateQueue main_queue(BEAM_WIDTH);

	PlaceLists place_lists(field, stones);

	std::vector<std::vector<std::vector<int>>> list2;
	if (stones_num % CLUSTER_SIZE == 0) {
		list2 = list1;
	}
	else {
		for (int i = 1; i <= cluster_size2; i++) {
			list2.push_back(ClusteredBeam::combination(cluster_size2, i));
		}
	}

	main_queue.push(field->clone_ex());

	for (int i = 0; i < max_cluster_num; i++) {
		//どの石を置くかのリスト
		auto list = list1;
		int cluster_size = CLUSTER_SIZE;
		if (i == max_cluster_num - 1) {
			list = list2;
			cluster_size = cluster_size2;
		}

		StateQueue tmp_main_queue(BEAM_WIDTH);

		//全ての状態に石を置くためのループ
		for (int st_idx = 0; st_idx < main_queue.size(); st_idx++) {
			auto state = main_queue[st_idx];
			tmp_main_queue.push(state->clone_ex());
			//クラスタ中の石の個数を変える為のループ
			for (int st_num = 1; st_num <= cluster_size; st_num++) {
				//nCrの石の選び方をすべて試すためのループ
				for (auto st_ncr : list[st_num - 1]) {
					//石の選び方に含まれる石をすべて置くためのループ
					for (int c_idx = 0; c_idx < st_ncr.size(); c_idx++) {
						int st_idx = CLUSTER_SIZE * i + st_ncr[c_idx];
						auto place_list = place_lists.get_list(st_idx);
						auto stone = stones[st_idx];
						bool pf = false;
						/*
						sharedなオブジェクト
						state, stone, place_list, tmp_queue
						*/
						int s = (int)(place_list.size());
						for (int j = 0; j < s; j++) {
							auto t = place_list[j];
							int r = std::get<0>(t);
							int a = std::get<1>(t);
							int x = std::get<2>(t);
							int y = std::get<3>(t);

							if (state->appliable_bit(stone, x, y, r, a)) {
								pf = true;
								auto tmp = state->clone_ex();
								tmp->apply_bit(stone, x, y, r, a);
								//std::cout << tmp->get_bit_str() << std::endl;

								tmp_main_queue.push(tmp);
							}
						}
						if (!pf) {
							if (state->eval_final_score() == result->eval_final_score()) {
								if (state->stone_count < result->stone_count) {
									result = state;
									std::cout << "result:" << result->eval_final_score() << std::endl;
								}
							}
							else if (state->eval_final_score() > result->eval_final_score()) {
								result = state;
								std::cout << "result:" << result->eval_final_score() << std::endl;
							}
						}
					}
				}
			}
		}
		main_queue = tmp_main_queue;
	}
	for (int i = 0; i < main_queue.size(); i++) {
		auto state = main_queue[i];
		if (state->eval_final_score() > result->eval_final_score()) {
			result = state;
			std::cout << "result:" << result->eval_final_score() << std::endl;
		}
	}
	std::ofstream ofs("answer.txt");
	ofs << HistoryTree::get_answer(result->parent_idx);
	ofs.close();

}

template <class F, class S>
std::shared_ptr<ExtendedField> ClusteredBeam<F, S>::solve(int cluster_num, std::vector<std::vector<int>> &values) {
	std::shared_ptr<ExtendedField> result = std::move(field->clone_ex());
	return result;
}

template <class F, class S>
std::vector<std::vector<int>> ClusteredBeam<F, S>::combination(int n, int r) {
	std::vector<std::vector<int>> values;
	std::vector<int> source;
	for (int i = 0; i < n; i++) {
		source.push_back(i);
	}
	std::vector<int> v(r);
	_combination(0, (int)(source.size()), r, source, 0, v, values);
	return values;
}

template <class F, class S>
void ClusteredBeam<F, S>::_combination(int begin, int size, int n, std::vector<int> &source, int index, std::vector<int> &v, std::vector<std::vector<int>> &values) {
	if (n == 0) {
//		v[index] = begin;
		std::vector<int> subset;
		for (std::vector<int>::iterator i = v.begin(); i != v.end(); i++) {
			subset.push_back(source[*i]);
		}
		values.push_back(subset);
	}
	else {
		for (int i = 0; i < size - n + 1; ++i) {
			v[index] = begin + i;
			_combination(begin + i + 1, size - i - 1, n - 1, source, index + 1, v, values);
		}
	}
}
