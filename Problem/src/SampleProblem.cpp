#include "SampleProblem.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

SampleProblem::SampleProblem(){

	std::ifstream ifs("problem/9.txt");
	if (ifs.fail()){
		std::cout << "failed" << std::endl;
	}
	std::string str;
	for (int i = 0; i < 32; i++){
		ifs >> str;
		field.push_back(str);
//		std::cout << str << std::endl;
	}
	/*
	ifs >> num;
	std::cout << num << std::endl;
	*/
	ifs >> str;
	num = std::atoi(str.c_str());
//	std::cout << num << std::endl;

	for (int i = 0; i < num; i++){
		std::vector<std::string> tmp_stone;
		for (int j = 0; j < 8; j++){
			ifs >> str;
			tmp_stone.push_back(str);
//			std::cout << str << std::endl;
		}
		stones.push_back(tmp_stone);

//		std::cout << std::endl;
	}
	/*
	std::cout << "test" << std::endl;
	std::cin >> str;
	*/
}


std::vector<std::string> SampleProblem::get_field_str() const
{
	return field;
}

std::vector<std::vector<std::string>> SampleProblem::get_stones_str() const
{
	return stones;
}

std::vector<std::string> SampleProblem::get_stone_str(int idx) const
{
	return stones[idx];
}

int SampleProblem::num_of_stones() const
{
	return num;
}

