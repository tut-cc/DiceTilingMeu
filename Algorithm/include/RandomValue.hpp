#pragma once
#include <random>

class RandomValue {
public:
	void test();
	static void init_random();
	//0�ȏ�1�ȉ��̏�����Ԃ�
	static float get_value();
private:
	static std::random_device rnd;
	static std::mt19937 mt;
	static std::uniform_real_distribution<float> dist;
};

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

