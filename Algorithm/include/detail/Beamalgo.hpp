#pragma once
#include "Beamalgo.hpp"
#include <iostream>

template <class F, class S>
Beamalgo<F, S>::Beamalgo(std::shared_ptr<Problem> p)
{
	field = std::move(std::unique_ptr<Field>(new F(p->get_field_str())));
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<Stone>(new S(s)));
	}
}
template <class F, class S>
static int Beamalgo<F, S>::h(std::shared_ptr<Stone> stone){
	return 0;
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

}

