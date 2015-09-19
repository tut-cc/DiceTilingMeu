#pragma once

#include "problem.hpp"
#include <memory>

template <class F, class S>
class Meu : public Algorithm {
private:
	std::unique_ptr<Field> field;
	std::vector<std::shared_ptr<Stone>> stones;

public:
	Meu(std::shared_ptr<Problem> p);
	virtual void solve() override;
};

#include "detail\/meu.hpp"
