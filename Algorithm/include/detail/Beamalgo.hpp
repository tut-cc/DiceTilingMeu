#pragma once
#include "Beamalgo.hpp"
#include <iostream>
#include <queue>

template <class F, class S>
Beamalgo<F, S>::Beamalgo(std::shared_ptr<Problem> p)
{
	field = std::move(std::unique_ptr<Field>(new F(p->get_field_str())));
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<Stone>(new S(s)));
	}
}


template <class F, class S>
void Beamalgo<F, S>::solve()
{
	/*
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			
			std::cout << field->at(i, j);
		}
		std::cout << std::endl;
	}

	for (int angle = 0; angle < 4; angle++){
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				std::cout << stones[0]->at(i, j, angle);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	*/
	
	std::vector<std::unique_ptr<Field>> s_list;
	s_list.push_back(field->clone());

	for (auto& st : stones){
		std::vector<std::shared_ptr<Field>> tmp_list;

		for (int si = 0; si < (int)(s_list.size());si++){
			std::unique_ptr<Field> s = s_list[si];
			for (int i = -7; i <= 7; i++){
				for (int j = -7; j <= 7; j++){
					for (int a = 0; a < 4; a++){
						for (int r = 0; r < 2; r++){
							if (s->appliable(st, i, j, r, a)){
								std::shared_ptr<Field> tmp = s->clone();
								tmp->apply(st, i, j, r, a);
								if (tmp_list.size() >= BEAM_WIDTH){
									tmp_list.pop_back();
								}
								tmp_list.push_back(tmp);
							}
						}
					}
				}
			}
		}
		
		s_list = tmp_list;
	}
	int size = (int)(s_list.size());
	for (int i = size - 1; i >= 0; i--){
		std::unique_ptr<Field> tmp = s_list[i];
		tmp->print();
		std::cout << std::endl;
	}
	
}