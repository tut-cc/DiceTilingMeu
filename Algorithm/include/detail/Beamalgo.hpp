#pragma once
#include "Beamalgo.hpp"
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

	//debug
	std::ofstream ofs("test_result.txt");

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
	std::priority_queue <
		std::shared_ptr<ExtendedField>,
		std::vector<std::shared_ptr<ExtendedField>>,
		std::function<bool(std::shared_ptr<ExtendedField>, std::shared_ptr<ExtendedField>) >> states_list(
			[](const std::shared_ptr<ExtendedField> &f1, const std::shared_ptr<ExtendedField> &f2) {
		return f1->eval_final_score() > f2->eval_final_score();
	});

	//std::priority_queue <
	//	std::shared_ptr<ExtendedField>,
	//	std::vector<std::shared_ptr<ExtendedField>>,
	//	std::function<bool(std::shared_ptr<ExtendedField>, std::shared_ptr<ExtendedField>) >> result_list(
	//		[](const std::shared_ptr<ExtendedField> &f1, const std::shared_ptr<ExtendedField> &f2) {
	//	return f1->eval_final_score() > f2->eval_final_score();
	//});

	std::cout << "field:\n" << field->get_bit_str() << std::endl;
	std::shared_ptr<ExtendedField> result = std::move(field->clone_ex());

	typedef std::vector<std::pair<int, int>> PairList;
	std::vector<std::array<std::array<PairList, 4>, 2>> appliable_list;
	for (int i = 0; i < stones_num; i++) {
		std::array<std::array<PairList, 4>, 2> tmp_st_list;
		for (int a = 0; a < 4; a++) {
			for (int r = 0; r < 2; r++) {
				PairList tmp_pl;
				for (int y = -7; y < 32; y++) {
					for (int x = -7; x < 32; x++) {
						if (field->appliable_bit(stones[i], x, y, r, a)) {
							tmp_pl.emplace_back(std::pair<int, int>(x, y));
						}
					}
				}
				tmp_st_list[r][a] = tmp_pl;
			}
		}
		appliable_list.push_back(tmp_st_list);
	}

	states_list.push(field->clone_ex());
	//全ての石を置く
	for (int st_idx = 0; st_idx < stones_num; st_idx++) {
		std::cout << st_idx << std::endl;
		std::shared_ptr<ExtendedStone> stone = stones[st_idx];

		std::priority_queue <
			std::shared_ptr<ExtendedField>,
			std::vector<std::shared_ptr<ExtendedField>>,
			std::function<bool(std::shared_ptr<ExtendedField>, std::shared_ptr<ExtendedField>) >> tmp_list(
				[](const std::shared_ptr<ExtendedField> &f1, const std::shared_ptr<ExtendedField> &f2) {
			return f1->eval_final_score() > f2->eval_final_score();
		});

		//全ての状態に対して石を置く
		while (!states_list.empty()) {
			std::shared_ptr<ExtendedField> state = states_list.top();
			states_list.pop();

			tmp_list.push(state->clone_ex());
			if (tmp_list.size() >= BEAM_WIDTH) {
				tmp_list.pop();
			}


//			std::cout << state << std::endl;

			//裏表、回転すべての置き方を試す
			for (int a = 0; a < 4; a++) {
				for (int r = 0; r < 1; r++) {

					for (auto& pos : appliable_list[st_idx][r][a]) {
						if (state->appliable_bit(stone, pos.first, pos.second, r, a)) {
							std::shared_ptr<ExtendedField> tmp = state->clone_ex();
							tmp->apply_bit(stone, pos.first, pos.second, r, a);
							//std::cout << tmp->get_bit_str() << std::endl;
							//キューのサイズをビーム幅に制限する
							tmp_list.push(tmp);
							if (tmp_list.size() >= BEAM_WIDTH) {
								tmp_list.pop();
							}
						}
						else {
							
							if (state->eval_final_score() > result->eval_final_score()) {
								result = state;
//								std::cout << "result:\n" << result->get_bit_str() << std::endl;
							}
						}
					}
				}
			}
		}
		states_list = tmp_list;
	}

	//結果を出力
	std::ofstream cv("cv.txt");
	cv << result->get_bit_str() << std::endl;
	std::ofstream ofs("answer.txt");
	//ofs << (*(result.get())) << std::endl;

	auto hst = result->get_history();
	if (hst.size() == 0)return;
	int t_id, t_x, t_y, t_r, t_a;
	std::shared_ptr<Stone> t_s;

	int b_id = -1;
	for (int i = 0; i < hst.size(); i++) {
		std::tie(t_s, t_x, t_y, t_r, t_a) = hst[i];
		t_id = t_s->identify();
		for (int j = b_id + 1; j < t_id; j++) { ofs << std::endl; }
		ofs << t_x << " " << t_y << " " << (t_r ? "T" : "H") << " " << (t_a * 90) << std::endl;
		b_id = t_id;
	}
	for (int j = t_id + 1; j < stones_num; j++) { ofs << std::endl; }
}