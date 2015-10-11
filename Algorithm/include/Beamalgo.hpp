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
	std::shared_ptr<ExtendedField> result;
	std::vector<std::shared_ptr<ExtendedStone>> stones;

	int stones_num;
	std::unique_ptr<Core> core;
	bool is_production;

	std::shared_ptr<ExtendedField> solve(int first_stone);
	void submit();
public:
	int BEAM_WIDTH = 1000;

	Beamalgo(std::shared_ptr<Problem> p, int bw);
	Beamalgo(std::shared_ptr<Problem> p, std::unique_ptr<Core> core, int bw);

	virtual ~Beamalgo() = default;
	virtual void solve();


};

#include "detail/Beamalgo.hpp"

