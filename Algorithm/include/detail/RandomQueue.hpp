#pragma once
#include <vector>
#include "ExtendedField.hpp"
#include "RandomValue.hpp"

class RandomQueue {
public:
	RandomQueue(int width);
	void push(std::shared_ptr < ExtendedField > &f);

	std::shared_ptr<ExtendedField> operator[](int index);
	int size();

private:
	std::vector<std::shared_ptr<ExtendedField>> queue;
	int width;
};

RandomQueue::RandomQueue(int width) : width(width) {}

inline void RandomQueue::push(std::shared_ptr<ExtendedField>& f)
{
#pragma omp critical
{
	if (queue.size() < width) {
		queue.push_back(f);
	}
	else {
		int idx = (int)(width * RandomValue::get_value());
		queue[idx] = std::move(f);
	}
}
}

inline std::shared_ptr<ExtendedField> RandomQueue::operator[](int index)
{
	return queue[index];
}

inline int RandomQueue::size()
{
	return (int)(queue.size());
}
