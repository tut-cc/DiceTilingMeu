#pragma once

#include <iostream>

template <class F, class S>
Meu<F, S>::Meu(std::shared_ptr<Problem> p)
{
	field = std::move(std::unique_ptr<Field>(new F(p->get_field_str())));
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<Stone>(new S(s, stones.size())));
	}
}


template <class F, class S>
void Meu<F, S>::solve()
{
	std::cout << "MeuMeuMeu" << std::endl;
}
