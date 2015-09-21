#pragma once

#include <iostream>

template <class F, class S>
Meu<F, S>::Meu(std::shared_ptr<Problem> p)
{
	field = std::move(std::unique_ptr<Field>(new F(p->get_field_str())));
	for (auto&& s : p->get_stones_str()) {
		stones.push_back(std::shared_ptr<Stone>(new S(s, stones.size())));
	}
	best_score = 1 << 28;

	for (auto && s : stones) {
		std::vector < std::tuple < int, int, int, int >> sub;
		for (int x = -8; x <= 32; ++x) {
			for (int y = -8; y <= 32; ++y) {
				for (int r = 0; r < 2; ++r) {
					for (int a = 0; a < 4; ++a) {
						if (field->appliable(s, x, y, r, a)) {
							sub.push_back(std::make_tuple(x, y, r, a));
						}
					}
				}
			}
		}
		std::cerr << "stone idx : " << s->identify() << "(" << sub.size() << ")" << std::endl;
		ok_list.push_back(sub);
	}
}

template <class F, class S>
void Meu<F, S>::solve() {
	dfs(field, 0);
	std::cout << *best;
}

template<class F, class S>
void Meu<F, S>::dfs(const std::unique_ptr<Field> & f, int depth) {
	if (best_score > f->score()) {
		best = std::move(f->clone());
		best_score = f->score();
	}
	if (depth == stones.size()) {
		return;
	}
	if (depth < 3) {
		std::cerr << "now : " << depth << "(apply)" << std::endl;
	}
	/**
	//for (int x = -8; x <= 32; ++x)
	//{
	//	for (int y = -8; y <= 32; ++y)
	//	{
	//		for (int ang = 0; ang < 4; ++ang)
	//		{
	//			for (int rev = 0; rev <= 1; ++rev)
	//			{
	//				bool can = f->appliable(stones[depth], x, y, rev, ang);
	//				if (can)
	//				{
	//					/* apply stone */
	//					/*auto dup = f->clone();
	//					dup->apply(stones[depth], x, y, rev, ang);
	//					dfs(dup, depth + 1);*/
	//					/* delete stone */
	//				}
	//			}
	//		}
	//	}
	//}
	/**/
	for (const auto & e : ok_list[depth]) {
		int x, y, r, a;
		std::tie(x, y, r, a) = e;
		if (f->appliable(stones[depth], x, y, r, a)) {
			auto dup = f->clone();
			dup->apply(stones[depth], x, y, r, a);
			dfs(dup, depth + 1);
		}
	}
	if (depth < 3) {
		std::cerr << "now : " << depth << "(NOT a@@lu)" << std::endl;
	}
	dfs(f, depth + 1);
}
