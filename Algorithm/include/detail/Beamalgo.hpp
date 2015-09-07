#pragma once
#include "Beamalgo.hpp"
#include <SimpleField.hpp>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>

template <class F, class S>
Beamalgo<F, S>::Beamalgo(std::shared_ptr<Problem> p)
{
	field = std::move(std::unique_ptr<ExtendedField>(new F(p->get_field_str())));
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<ExtendedStone>(new S(s, (int)(stones.size()))));
	}
	stones_num = (int)(stones.size());
}


template <class F, class S>
void Beamalgo<F, S>::solve()
{
	std::priority_queue <
		std::shared_ptr<ExtendedField>,
		std::vector<std::shared_ptr<ExtendedField>>,
		std::function<bool(std::shared_ptr<ExtendedField>, std::shared_ptr<ExtendedField>) >> states_list(
			[](const std::shared_ptr<ExtendedField> &f1, const std::shared_ptr<ExtendedField> &f2) {
		return f1->eval_select_score() < f2->eval_select_score();
	});

	//std::priority_queue <
	//	std::shared_ptr<ExtendedField>,
	//	std::vector<std::shared_ptr<ExtendedField>>,
	//	std::function<bool(std::shared_ptr<ExtendedField>, std::shared_ptr<ExtendedField>) >> result_list(
	//		[](const std::shared_ptr<ExtendedField> &f1, const std::shared_ptr<ExtendedField> &f2) {
	//	return f1->eval_final_score() > f2->eval_final_score();
	//});

	std::shared_ptr<ExtendedField> result = std::move(field->clone_ex());

	typedef std::vector<std::pair<int, int>> PairList;
	std::vector<std::array<std::array<PairList, 4>, 2>> appliable_list;
	for (int i = 0; i < stones_num; i++) {
		std::array<std::array<PairList, 4>, 2> tmp_st_list;
		for (int a = 0; a < 4; a++) {
			for (int r = 0; r < 1; r++) {
				PairList tmp_pl;
				for (int y = -7; y < 32; y++) {
					for (int x = -7; x < 32; x++) {
						if (field->appliable_ex(stones[i], x, y, r, a)) {
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
		std::shared_ptr<ExtendedStone> stone = stones[st_idx];

		std::priority_queue <
			std::shared_ptr<ExtendedField>,
			std::vector<std::shared_ptr<ExtendedField>>,
			std::function<bool(std::shared_ptr<ExtendedField>, std::shared_ptr<ExtendedField>) >> tmp_list(
				[](const std::shared_ptr<ExtendedField> &f1, const std::shared_ptr<ExtendedField> &f2) {
			return f1->eval_select_score() < f2->eval_select_score();
		});

		//全ての状態に対して石を置く
		while (!states_list.empty()) {
			std::shared_ptr<ExtendedField> state = states_list.top();
			states_list.pop();

			if (tmp_list.size() >= BEAM_WIDTH) {
				tmp_list.pop();
			}
			tmp_list.push(state->clone_ex());

			//裏表、回転すべての置き方を試す
			for (int a = 0; a < 4; a++) {
				for (int r = 0; r < 1; r++) {

					for (auto& pos : appliable_list[st_idx][r][a]) {
						if (state->appliable_ex(stone, pos.first, pos.second, r, a)) {
							std::shared_ptr<ExtendedField> tmp = state->clone_ex();
							tmp->apply_ex(stone, pos.first, pos.second, r, a);

							//キューのサイズをビーム幅に制限する
							if (tmp_list.size() >= BEAM_WIDTH) {
								tmp_list.pop();
							}
							tmp_list.push(tmp);
						}
						else {
							if (state->eval_final_score() > result->eval_final_score()) {
								result = state;
							}
						}
					}
				}
			}
		}
		states_list = tmp_list;
	}

	std::cout << result << std::endl;
}