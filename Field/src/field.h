
#include "../../Stone/include/stone.h"
#include <string>
#define ST 1
#define NO 0
class Field {
private:
	int field[32][32];
	
public:
	Field(std::string str){
	}
	
	Field(){
	}
	virtual bool at(int x, int y) = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
};

