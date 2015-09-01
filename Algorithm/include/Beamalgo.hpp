#pragma once

#include "algorithm.hpp"
#include "field.hpp"
#include "stone.hpp"
#include "problem.hpp"
#include <memory>
#include <vector>

template <class F, class S>
class Beamalgo : public Algorithm {
	std::unique_ptr<Field> field;
	std::vector<std::shared_ptr<Stone>> stones;

	const int BEAM_WIDTH = 10;
public:
	Beamalgo(std::shared_ptr<Problem> p);

	virtual ~Beamalgo() = default;
	virtual void solve();
};

#include "detail/Beamalgo.hpp"

