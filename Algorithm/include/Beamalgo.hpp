#pragma once

#include "algorithm.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "problem.hpp"
#include "ClusteredBeam.hpp"
#include "Core.hpp"
#include <memory>
#include <vector>

template <class F, class S>
class Beamalgo : public Algorithm {
	std::shared_ptr<ExtendedField> field;
	std::vector<std::shared_ptr<ExtendedStone>> stones;

	int stones_num;
	const int BEAM_WIDTH = 1000;
	std::unique_ptr<Core> core;
	bool is_production;

	std::shared_ptr<ExtendedField> solve(int first_stone);
	void submit(std::unique_ptr<Field>);
public:
	Beamalgo(std::shared_ptr<Problem> p);
	Beamalgo(std::shared_ptr<Problem> p, std::unique_ptr<Core> core);

	virtual ~Beamalgo() = default;
	virtual void solve();


};

#include "detail/Beamalgo.hpp"

