#pragma once
#include "Beamalgo.hpp"
#include <SimpleField.hpp>
#include <iostream>
#include <queue>

template <class F, class S>
Beamalgo<F, S>::Beamalgo(std::shared_ptr<Problem> p)
{
	field = std::move(std::unique_ptr<Field>(new F(p->get_field_str())));
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<Stone>(new S(s, (int)(stones.size()))));
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
	
	std::priority_queue<std::unique_ptr<Field>> result_list;

	std::priority_queue<std::unique_ptr<Field>> s_list;
	s_list.push(field->clone());
	
	
	for (auto& st : stones){
		std::priority_queue<std::unique_ptr<Field>> tmp_list;
		
		for (int si = 0; si < (int)(s_list.size());si++){
//			std::unique_ptr<Field> s = std::move(s_list.top());
			std::unique_ptr<Field> s = s_list.top()->clone();
			s_list.pop();
			
			for (int i = -7; i < 32; i++){
				for (int j = -7; j < 32; j++){
					for (int a = 0; a < 4; a++){
						for (int r = 0; r < 2; r++){
							
							if (s->appliable(st, i, j, r, a)){
								
								std::unique_ptr<Field> tmp = s->clone();
								tmp->apply(st, i, j, r, a);
								
								if (tmp_list.size() >= BEAM_WIDTH){
									tmp_list.pop();
								}
								tmp_list.push(std::move(tmp));
							}
							else{
								if (result_list.size() >= 10){
									result_list.pop();
								}
								result_list.push(s->clone());
							}
						}
					}
				}
			}
			
		}

		s_list = std::move(tmp_list);
	}
	
	
	int size = (int)(result_list.size());
	for (int i = size - 1; i >= 0; i--){
		std::unique_ptr<Field> tmp = result_list.top()->clone();

		for (int i = 0; i < 32; i++){
			for (int j = 0; j < 32; j++){

				std::cout << tmp->at(i, j);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		result_list.pop();
	}
	
}

bool operator <(SimpleField f1, SimpleField f2){
	return f1.h() < f2.h();
}
bool operator >(SimpleField f1, SimpleField f2){
	return f1.h() > f2.h();
}