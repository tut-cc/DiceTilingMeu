#pragma once
#include "Beamalgo.hpp"
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
	RandomValue::init_random();

//	std::cout << field->eval_select_score() << "\n" << field->get_bit_str() << std::endl;
}


template <class F, class S>
void Beamalgo<F, S>::solve()
{
	std::cout << "field:\n" << field->get_bit_str() << std::endl;
	std::shared_ptr<ExtendedField> result = std::move(field->clone_ex());

	for (int i = 0; i < stones_num-1; i++) {
		HistoryTree::clear();
		auto tmp = std::move(solve(i));
		if (tmp->eval_final_score() > result->eval_final_score()) {
			result = std::move(tmp);
			std::cout << "score:" << result->eval_final_score() << std::endl;
			std::ofstream ofs("answer.txt");
			ofs << HistoryTree::get_answer(result->parent_idx);
			ofs.close();

		}
	}

	//���ʂ��o��
	std::ofstream cv("cv.txt");
	cv << result->get_bit_str() << std::endl;
}

template<class F, class S>
std::shared_ptr<ExtendedField> Beamalgo<F, S>::solve(int first_stone) {
	StateQueue main_queue(BEAM_WIDTH);
	RandomQueue sub_queue(BEAM_WIDTH);

	std::shared_ptr<ExtendedField> result = std::move(field->clone_ex());

	PlaceLists place_lists(field, stones);

	sub_queue.push(field->clone_ex());
	//�S�Ă̐΂�u��
	for (int st_idx = first_stone; st_idx < stones_num; st_idx++) {
		std::cout << st_idx << std::endl;
		std::shared_ptr<ExtendedStone> stone = stones[st_idx];

		StateQueue tmp_main_queue(BEAM_WIDTH);
		RandomQueue tmp_sub_queue(BEAM_WIDTH);

		//�S�Ă̏�Ԃɑ΂��Đ΂�u��
		for (int i = 0; i < main_queue.size(); i++) {
			auto state = main_queue[i];
			if(i != 0)tmp_sub_queue.push(state->clone_ex());

			auto place_list = place_lists.get_list(st_idx);

			bool pf = false;
			/*
			shared�ȃI�u�W�F�N�g
			state, stone, place_list, tmp_queue
			*/
			int s = place_list.size();
#pragma omp parallel for shared(state, stone, tmp_sub_queue, tmp_main_queue, result, place_list)
			for (int j = 0; j < s; j++) {
				//			for (auto t : place_list) {
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
				if (state->eval_final_score() > result->eval_final_score()) {
					result = state;
					//std::cout << "result:\n" << result->get_bit_str() << std::endl;
				}
			}

		}
		//�S�Ă̏�Ԃɑ΂��Đ΂�u��
#pragma omp parallel for shared(stone, tmp_sub_queue, sub_queue, tmp_main_queue, main_queue, result, place_list)
		for (int i = 0; i < sub_queue.size(); i++) {
			auto state = sub_queue[i];
			tmp_sub_queue.push(state->clone_ex());

			auto place_list = place_lists.get_list(st_idx);

			bool pf = false;
			/*
			shared�ȃI�u�W�F�N�g
			state, stone, place_list, tmp_queue
			*/
			int s = place_list.size();
			for (int j = 0; j < s; j++) {
				//			for (auto t : place_list) {
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
				if (state->eval_final_score() > result->eval_final_score()) {
					result = state;
					//std::cout << "result:\n" << result->get_bit_str() << std::endl;
				}
			}
		}

		main_queue = tmp_main_queue;
		sub_queue = tmp_sub_queue;
	}

	return result;
}
