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
virtual void Beamalgo<F, S>::solve()
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
	std::priority_queue<std::shared_ptr<Field>> s_list;
	std::shared_ptr<Field> s_0 = field;
	s_list.push_back(s_0);

	for (auto& st : stones){
		for (auto& s : s_list){
			for (int i = -7; i <= 7; i++){
				for (int j = -7; j <= 7; j++){
					for (int a = 0; a < 4; a++){
						if (s.appliable(st, i, j, a)){
							s.apply(st, i, j, a);
						}
					}
				}
			}
		}
	}
}

