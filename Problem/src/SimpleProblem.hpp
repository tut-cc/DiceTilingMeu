#pragma once

#include "problem.h"

template <class F, class S>
class SimpleProblem : public Problem<F, S> {
private:
	std::unique_ptr<Field> field;
	std::vector<std::shared_ptr<Stone>> stones;

public:
	SimpleProblem();
	virtual ~SimpleProblem() {};
	virtual std::unique_ptr<Field> get_field();
	virtual std::vector<std::weak_ptr<Stone>> get_stones();
};
