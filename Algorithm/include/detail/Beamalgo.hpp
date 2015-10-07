#pragma once
#include "Beamalgo.hpp"
#include "PlaceLists.hpp"
#include "StateQueue.hpp"
#include "RandomValue.hpp"
#include <SimpleField.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <fstream>

template <class F, class S>
Beamalgo<F, S>::Beamalgo(std::shared_ptr<Problem> p)
{
	field = std::move(std::shared_ptr<ExtendedField>(new F(p->get_field_str())));

	int s_idx = 0;
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<ExtendedStone>(new S(s, s_idx)));
		s_idx++;
	}
	stones_num = (int)(stones.size());
	HistoryTree::init_history_tree(stones_num);
	

	//for (auto st : stones) {
	//	std::cout << "#" << st->identify() << std::endl;
	//	auto pl = st->get_place_list();
	//	std::cout << "type:" << pl.size() << std::endl;
	//	for (auto p : pl) {
	//		std::cout << st->get_bit_str(p.first, p.second) << "\n" << std::endl;
	//	}
	//}

	//debug
	//std::ofstream ofs("test_result.txt");

	//for (int r = 0; r < 2; r++) {
	//	for (int a = 0; a < 4; a++) {
	//		ofs << stones[0]->get_bit_str(r, a) << "\n" << std::endl;
	//		for (int y = -7; y < 32; y++) {
	//			for (int x = -7; x < 32; x++) {
	//				if (field->appliable_bit(stones[0], x, y, r, a)) {
	//					auto tmp = field->clone_ex();
	//					tmp->apply_bit(stones[0], x, y, r, a);
	//					ofs << x << " " << y << " " << r << " " << a << std::endl;
	//					ofs << tmp->get_bit_str() << std::endl;
	//					ofs << "\n";
	//				}
	//			}
	//		}
	//	}
	//}
	//std::cout << stones[0]->get_bit_str(0, 0) << "\n" << std::endl;
	//std::cout << stones[0]->get_neighbor_str(0, 0) << "\n" << std::endl;
	//std::cout << stones[1]->get_bit_str(0, 0) << "\n" << std::endl;
	//std::cout << stones[1]->get_neighbor_str(0, 0) << "\n" << std::endl;

	//std::cout << field->appliable_bit(stones[0], -1, 0, 0, 0) << std::endl;
	//field->apply_bit(stones[0], -1, 0, 0, 0);
	//std::cout << field->get_bit_str() << std::endl;
	//std::cout << "\n" << field->get_neighbor_str() << std::endl;

	//std::cout << field->appliable_bit(stones[1], 1, 0, 0, 0) << std::endl;
	//field->apply_bit(stones[1], 1, 0, 0, 0);
	//std::cout << field->get_bit_str() << std::endl;
	//std::cout << "\n" << field->get_neighbor_str() << std::endl;

}


template <class F, class S>
void Beamalgo<F, S>::solve()
{
	StateQueue queue(BEAM_WIDTH);

	std::cout << "field:\n" << field->get_bit_str() << std::endl;
	std::shared_ptr<ExtendedField> result = std::move(field->clone_ex());

	PlaceLists place_lists(field, stones);

	queue.push(field->clone_ex());
	//全ての石を置く
	for (int st_idx = 0; st_idx < stones_num; st_idx++) {
		std::cout << st_idx << std::endl;
		std::shared_ptr<ExtendedStone> stone = stones[st_idx];

		StateQueue tmp_queue(BEAM_WIDTH);

		//全ての状態に対して石を置く
		for (int i = 0; i < queue.size(); i++) {
			auto state = queue[i];
			tmp_queue.push(state->clone_ex());

			auto place_list = place_lists.get_list(st_idx);

			/*
			sharedなオブジェクト
			state, stone, place_list, tmp_queue
			*/
			int s = place_list.size();
#pragma omp parallel for shared(state, stone, tmp_queue, result)
			for (int j = 0; j < s;j++){
//			for (auto t : place_list) {
				auto t = place_list[j];
				int r = std::get<0>(t);
				int a = std::get<1>(t);
				int x = std::get<2>(t);
				int y = std::get<3>(t);

				{
					if (state->appliable_bit(stone, x, y, r, a)) {
						auto tmp = state->clone_ex();
						tmp->apply_bit(stone, x, y, r, a);
						//std::cout << tmp->get_bit_str() << std::endl;

						tmp_queue.push(tmp);
					}
					else {
						if (state->eval_final_score() > result->eval_final_score()) {
							result = state;
							//std::cout << "result:\n" << result->get_bit_str() << std::endl;
						}
					}
				}
			}

		}
		queue = tmp_queue;
	}

	//結果を出力
	std::ofstream cv("cv.txt");
	cv << result->get_bit_str() << std::endl;
	std::ofstream ofs("answer.txt");

	ofs << HistoryTree::get_answer(result->parent_idx);
}