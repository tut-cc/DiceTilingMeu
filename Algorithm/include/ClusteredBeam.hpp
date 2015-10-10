#pragma once

#include "algorithm.hpp"
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"
#include "problem.hpp"
#include <memory>
#include <vector>

template <class F, class S>
class ClusteredBeam: public Algorithm {
	std::shared_ptr<ExtendedField> field;
	std::vector<std::shared_ptr<ExtendedStone>> stones;

	int stones_num;

	const int BEAM_WIDTH = 1000;
	const int CLUSTER_SIZE = 3;

	std::shared_ptr<ExtendedField> solve(int cluster_num, std::vector<std::vector<int>> &stones);
	static void _combination(int begin, int size, int n, std::vector<int> &source, int index, std::vector<int> &v, std::vector<std::vector<int>> &values);

public:
	ClusteredBeam(std::shared_ptr<Problem> p);

	static std::vector<std::vector<int>> combination(int n, int r);

	virtual ~ClusteredBeam() = default;
	virtual void solve();
};

#include "detail/ClusteredBeam.hpp"

