#pragma once
#include <queue>
#include "ExtendedField.hpp"

class StateQueue {
public:
	StateQueue(int max_width);
	void push(std::shared_ptr<ExtendedField> &f);

	std::shared_ptr<ExtendedField> operator[](int index);

	int size();
private:
	int max_width;
	std::vector<std::shared_ptr<ExtendedField>> vec;

	void remove_min();
	void create_heap();
};

StateQueue::StateQueue(int max_width) : max_width(max_width) {}

inline void StateQueue::push(std::shared_ptr<ExtendedField> &f) {
#pragma omp critical
{
	if (vec.size() == max_width) {
		remove_min();
	}
	vec.push_back(f);
	create_heap();
}
}

inline void StateQueue::remove_min()
{
	int n = vec.size() - 1;
	vec[0] = vec[n];
	auto tmp = vec.back();
	HistoryTree::remove(tmp->parent_idx);
	vec.pop_back();
	for (int i = 0, j; (j = 2 * i + 1) < n;) {
		if ((j != n - 1) && (vec[j]->eval_select_score() < vec[j + 1]->eval_select_score())) {
			j++;
		}
		if (vec[i]->eval_select_score() < vec[j]->eval_select_score()) {
			auto tmp = std::move(vec[j]);
			vec[j] = std::move(vec[i]);
			vec[i] = std::move(tmp);
		}
		i = j;
	}
}
inline void StateQueue::create_heap()
{
	int n = vec.size() - 1;
	while (n != 0){
		int i = (n - 1) / 2;
		if (vec[n]->eval_select_score() > vec[i]->eval_select_score()) {
			auto tmp = std::move(vec[n]);
			vec[n] = std::move(vec[i]);
			vec[i] = std::move(tmp);
		}
		n = i;
	}

}

std::shared_ptr<ExtendedField> StateQueue::operator[](int index) {
	return vec.at(index);
}

inline int StateQueue::size()
{
	return vec.size();
}
