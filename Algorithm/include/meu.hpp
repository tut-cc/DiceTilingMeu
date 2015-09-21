#pragma once

#include "problem.hpp"
#include <memory>

template <class F, class S>
class Meu : public Algorithm {
private:
	std::unique_ptr<Field> field;
	std::vector<std::shared_ptr<Stone>> stones;
	std::vector <std::vector < std::tuple < int, int, int, int >>> ok_list;

	std::unique_ptr<Field> best;
	int best_score;
	void dfs(const std::unique_ptr<Field> &, int depth);
public:
	Meu(std::shared_ptr<Problem> p);
	virtual void solve() override;
};


#include "detail\/meu.hpp"
