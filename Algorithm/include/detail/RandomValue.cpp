#include "RandomValue.hpp"


void RandomValue::init_random()
{
	mt.seed(rnd());
	dist = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

float RandomValue::get_value() {
	return dist(mt);
}

std::random_device RandomValue::rnd;
std::mt19937 RandomValue::mt;
std::uniform_real_distribution<float> RandomValue::dist;


