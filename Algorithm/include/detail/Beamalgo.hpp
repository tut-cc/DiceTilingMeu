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

	std::priority_queue <
		std::shared_ptr<ExtendedField>,
		std::vector<std::shared_ptr<ExtendedField>>,
		std::function<bool(std::shared_ptr<ExtendedField>, std::shared_ptr<ExtendedField>) >> result_list(
			[](const std::shared_ptr<ExtendedField> &f1, const std::shared_ptr<ExtendedField> &f2) {
		return f1->eval_final_score() < f2->eval_final_score();
	});

	typedef std::vector<std::pair<int, int>> PairList;
	std::vector<std::array<std::array<PairList, 4>, 2>> appliable_list;
	for (int i = 0; i < stones_num; i++) {
		std::array<std::array<PairList, 4>, 2> tmp_st_list;
		for (int a = 0; a < 4; a++) {
			for (int r = 0; r < 1; r++) {
				PairList tmp_pl;
				for (int y = -7; y < 32; y++) {
					for (int x = -7; x < 32; x++) {
						if (field->appliable(stones[i], x, y, r, a)) {
							tmp_pl.emplace_back(std::pair<int, int>(x, y));
						}
					}
				}
				tmp_st_list[r][a] = tmp_pl;
			}
		}
		appliable_list.push_back(tmp_st_list);
	}
/*
//	std::priority_queue<std::unique_ptr<Field>> result_list;
//
//	std::priority_queue<std::unique_ptr<Field>> s_list;
//	s_list.push(field->clone());
//
//	std::vector<std::array<std::array<std::vector<std::pair<int, int>>, 2>, 4>> appliable_list;
//	int len = (int)(stones.size());
//	for (int i = 0; i < len; i++) {
//		std::shared_ptr<Stone> s = stones[i];
//		std::array<std::array<std::vector<std::pair<int, int>>, 2>, 4> tmp_vec1;
//		for (int r = 0; r < 4; r++) {
//			for (int a = 0; a < 2; a++) {
//				std::vector<std::pair<int, int>> tmp_vec2;
//				for (int y = -7; y < 32; y++) {
//					for (int x = -7; x < 32; x++) {
//						if (field->appliable(s, x, y, r, a)) {
//							tmp_vec2.emplace_back(std::pair<int, int>(x, y));
//						}
//					}
//				}
//				tmp_vec1[r][a] = tmp_vec2;
//			}
//		}
//		appliable_list.push_back(tmp_vec1);
//	}
*/

	states_list.push(field->clone_ex());
	//全ての石を置く
	for (int st_idx = 0; st_idx < stones_num; st_idx++) {
		//全ての状態に対して石を置く
		while (!states_list.empty()){
			auto state = states_list.top();
			states_list.pop();
			//裏表、回転すべての置き方を試す
			for (int a = 0; a < 4; a++) {
				for (int r = 0; r < 1; r++) {

					for (auto& pos : appliable_list[st_idx][r][a]) {
						
					}
				}
			}
		}
	}
//	
//	for (auto& st : stones){
//		std::priority_queue<std::unique_ptr<Field>> tmp_list;
//		
//		for (int si = 0; si < (int)(s_list.size());si++){
////			std::unique_ptr<Field> s = std::move(s_list.top());
//			std::unique_ptr<Field> s = s_list.top()->clone();
//			s_list.pop();
//
//			for (int a = 0; a < 4; a++){
//				for (int r = 0; r < 2; r++) {
//					for (auto& loc_list : appliable_list.at(si)[a][r]) {
//						int i = loc_list.first;
//						int j = loc_list.second;
//						if (s->appliable(st, i, j, r, a)) {
//
//							std::unique_ptr<Field> tmp = s->clone();
//							tmp->apply(st, i, j, r, a);
//
//							if (tmp_list.size() >= BEAM_WIDTH) {
//								tmp_list.pop();
//							}
//							tmp_list.push(std::move(tmp));
//						}
//						else {
//							if (result_list.size() >= 10) {
//								result_list.pop();
//							}
//							result_list.push(s->clone());
//						}
//					}
//				}
//
//			}
//			
//		}
//
//		s_list = std::move(tmp_list);
//	}
//	
//	
//	int size = (int)(result_list.size());
//	for (int i = size - 1; i >= 0; i--){
//		std::unique_ptr<Field> tmp = result_list.top()->clone();
//
//		for (int i = 0; i < 32; i++){
//			for (int j = 0; j < 32; j++){
//
//				std::cout << tmp->at(i, j);
//			}
//			std::cout << std::endl;
//		}
//		std::cout << std::endl;
//		result_list.pop();
//	}
//	
}

