//�΂�u���Ֆʂ̃N���X
#include "../../Stone/include/stone.h"
#include <string>
#define ST 1
#define NO 0
class Field {
private:
	int field[32][32];
	
public:
	//���Ɠ��l�̌`���̕����񂪓n�����
	Field(std::string str){
	}
	
	//�W�����͂���Field���쐬����
	Field(){
	}
	virtual bool at(int x, int y) = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
};

