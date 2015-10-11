#pragma once
#include "Beamalgo.hpp"
#include "PlaceLists.hpp"
#include "StateQueue.hpp"
#include "RandomValue.hpp"
#include "RandomQueue.hpp"
#include "Core.hpp"
#include <SimpleField.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <fstream>

template <class F, class S>
Beamalgo<F, S>::Beamalgo(std::shared_ptr<Problem> p, std::unique_ptr<Core> core, int bw)
{
	BEAM_WIDTH = bw;
	field = std::move(std::shared_ptr<ExtendedField>(new F(p->get_field_str())));

	int s_idx = 0;
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<ExtendedStone>(new S(s, s_idx)));
		s_idx++;
	}
	stones_num = (int)(stones.size());
	HistoryTree::init_history_tree(stones_num);
	RandomValue::init_random();

	this->core = std::move(core);
	is_production |= true;

}
template <class F, class S>
Beamalgo<F, S>::Beamalgo(std::shared_ptr<Problem> p, int bw)
{
	BEAM_WIDTH = bw;
	field = std::move(std::shared_ptr<ExtendedField>(new F(p->get_field_str())));

	int s_idx = 0;
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<ExtendedStone>(new S(s, s_idx)));
		s_idx++;
	}
	stones_num = (int)(stones.size());
	HistoryTree::init_history_tree(stones_num);
	RandomValue::init_random();

	//int n = 5;
	//for (int i = 1; i <= n; i++) {
	//	std::cout << "[" << n <<  ", " << i << "]" << std::endl;
	//	auto values = ClusteredBeam<F, S>::combination(n, i);
	//	for (auto val : values) {
	//		for (auto e : val) {
	//			std::cout << e << " ";
	//		}
	//		std::cout << std::endl;
	//	}
	//	std::cout << std::endl;
	//}
}


template <class F, class S>
void Beamalgo<F, S>::solve()
{
	std::cout << "field:\n" << field->get_bit_str() << std::endl;
	result = std::move(field->clone_ex());

	for (int i = 0; i * i < stones_num-1; i++) {
//	for (int i = 0; i < 1; i++) {
		HistoryTree::clear();
		auto tmp = std::move(solve(i));
		std::cout << "now score:" << result->get_final_score() << " " << result->stone_count << std::endl;
		std::cout << "new score:" << tmp->get_final_score() << " " << tmp->stone_count << std::endl;

		answer_output(tmp);

		if (tmp->eval_final_score() > result->eval_final_score() ||
			(tmp->eval_final_score() == result->eval_final_score() && tmp->stone_count < result->stone_count)) {
			std::cout << "score updated:" << tmp->get_final_score() << " " << tmp->stone_count << std::endl;
			result = std::move(tmp);
			submit();
			std::cout << "answer complete" << std::endl;
		}
	}

	//結果を出力
	std::ofstream cv("cv.txt");
	cv << result->get_bit_str() << std::endl;
}

template <class F, class S>
void Beamalgo<F, S>::submit()
{
	if (is_production) {
		core->submit(ExtendedField::space_count - result->get_final_score(), result->stone_count, "answer.txt");
	}
}


template<class F, class S>
std::shared_ptr<ExtendedField> Beamalgo<F, S>::solve(int first_stone) {
	StateQueue main_queue(BEAM_WIDTH);
	RandomQueue sub_queue(BEAM_WIDTH);

	std::shared_ptr<ExtendedField> e_result = std::move(field->clone_ex());

	PlaceLists place_lists(field, stones);

	sub_queue.push(field->clone_ex());
	//全ての石を置く
	for (int st_idx = first_stone; st_idx < stones_num; st_idx++) {
		std::cout << st_idx << std::endl;
		std::shared_ptr<ExtendedStone> stone = stones[st_idx];

		StateQueue tmp_main_queue(BEAM_WIDTH);
		RandomQueue tmp_sub_queue(BEAM_WIDTH);

		//全ての状態に対して石を置く
#pragma omp parallel for shared(stone, tmp_sub_queue, sub_queue, tmp_main_queue, main_queue, e_result, place_lists)
		for (int i = 0; i < main_queue.size(); i++) {
			auto state = main_queue[i];
			if(i != 0)tmp_sub_queue.push(state->clone_ex());

			//if (i == 0) {
			//	std::cout << "tmp_score…" << state->eval_final_score() << std::endl;
			//	std::cout << "tmp_score…\n" << state->get_bit_str() << std::endl;
			//}

			auto place_list = place_lists.get_list(st_idx);

			bool pf = false;
			/*
			sharedなオブジェクト
			state, stone, place_list, tmp_queue
			*/
			int s = (int)(place_list.size());
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
				if (state->eval_final_score() == e_result->eval_final_score()) {
					if (state->stone_count < e_result->stone_count) {
						e_result = state;
						std::cout << "result:" << e_result->get_final_score() << " " << e_result->stone_count << std::endl;
					}
				}
				else if (state->eval_final_score() > e_result->eval_final_score()) {
					e_result = state;
					std::cout << "result:" << e_result->get_final_score() << " " << e_result->stone_count << std::endl;
				}
			}

		}
		//全ての状態に対して石を置く
#pragma omp parallel for shared(stone, tmp_sub_queue, sub_queue, tmp_main_queue, main_queue, e_result, place_lists)
		for (int i = 0; i < sub_queue.size(); i++) {
			auto state = sub_queue[i];
			tmp_sub_queue.push(state->clone_ex());

			auto place_list = place_lists.get_list(st_idx);

			bool pf = false;
			/*
			sharedなオブジェクト
			state, stone, place_list, tmp_queue
			*/
			int s = (int)(place_list.size());
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
				if (state->eval_final_score() == e_result->eval_final_score()) {
					if (state->stone_count < e_result->stone_count) {
						e_result = state;
						std::cout << "result:" << e_result->get_final_score() << " " << e_result->stone_count << std::endl;
					}
				}
				else if (state->eval_final_score() > e_result->eval_final_score()) {
					e_result = state;
					std::cout << "result:" << e_result->get_final_score() << " " << e_result->stone_count << std::endl;
				}
			}
		}

		main_queue = tmp_main_queue;
		sub_queue = tmp_sub_queue;
	}
	for (int i = 0; i < main_queue.size(); i++) {
		auto state = main_queue[i];
		if (state->eval_final_score() > e_result->eval_final_score()) {
			e_result = state;
			std::cout << "result:" << e_result->get_final_score() << " " << e_result->stone_count << std::endl;
		}
	}
	for (int i = 0; i < sub_queue.size(); i++) {
		auto state = sub_queue[i];
		if (state->eval_final_score() > e_result->eval_final_score()) {
			e_result = state;
			std::cout << "result:" <<e_result->get_final_score() << " " << e_result->stone_count << std::endl;
		}
	}

	return e_result;
}

template <class F, class S>
void Beamalgo<F, S>::answer_output(std::shared_ptr<ExtendedField> &r) {
	boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
	std::string fn = "tmp_answer/" + boost::posix_time::to_iso_string(time) + ".txt";

	std::ofstream ofs(fn);
	ofs << HistoryTree::get_answer(r->parent_idx);
	ofs.close();

	std::cout << "saved:" << r->get_final_score() << " " << r->stone_count << "\n" << fn << std::endl;
}