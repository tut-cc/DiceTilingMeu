//�΂�u���Ֆʂ̃N���X
#include "../Stone/stone.h"

#define ST 1
#define NO 0
class Field {
private:
	int[][] field;
	
public:
	//���Ɠ��l�̌`���̕����񂪓n�����
	Field(string str){
	}
	
	//�W�����͂���Field���쐬����
	Field(){
	}
	virtual bool at(int x, int y) = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
};

