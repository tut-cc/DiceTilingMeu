//quest11.txt‚©‚ç–â‘è‚ğ“Ç‚İ‚ß‚é‚æ‚¤‚É‚µ‚½‚¾‚¯‚ÌSimpleProblem
#pragma once

#include "problem.hpp"

class SampleProblem : public Problem {
private:
	std::vector<std::string> field;
	int num;
	std::vector<std::vector<std::string>> stones;

public:
	SampleProblem();
	virtual ~SampleProblem() = default;
	virtual std::vector<std::string> get_field_str() const;
	virtual std::vector<std::vector<std::string>> get_stones_str() const;
	virtual std::vector<std::string> get_stone_str(int idx) const;
	virtual int num_of_stones() const;
};

