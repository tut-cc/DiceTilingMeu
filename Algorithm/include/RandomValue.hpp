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
