#pragma once
#include "stone.h"
#include <memory>

class Field {
private:
	
public:
	static const int NO = 0;
	static const int ST = 1;
	Field(string str){
	}
	
	Field(){
	}
	virtual bool at(int x, int y) = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
	std::unique_ptr<Field> clone() = 0;
};

