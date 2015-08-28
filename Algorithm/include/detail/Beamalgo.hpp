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
void Beamalgo<F, S>::solve()
{
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++){
			
			std::cout << field->at(i, j);
		}
		std::cout << std::endl;
	}
}

