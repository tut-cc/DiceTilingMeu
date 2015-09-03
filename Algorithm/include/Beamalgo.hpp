#pragma once

#include "algorithm.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "problem.hpp"
#include <memory>
#include <vector>

template <class F, class S>
class Beamalgo : public Algorithm {
	std::unique_ptr<ExtendedField> field;
	std::vector<std::shared_ptr<ExtendedStone>> stones;

	int stones_num;

	const int BEAM_WIDTH = 10;
public:
	Beamalgo(std::shared_ptr<Problem> p);

	virtual ~Beamalgo() = default;
	virtual void solve();

};

#include "detail/Beamalgo.hpp"

