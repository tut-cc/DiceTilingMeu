//石を置く盤面のクラス
#include "../../Stone/include/stone.h"
#include <string>
#define ST 1
#define NO 0
class Field {
private:
	int field[32][32];
	
public:
	//問題と同様の形式の文字列が渡される
	Field(std::string str){
	}
	
	//標準入力からFieldを作成する
	Field(){
	}
	virtual bool at(int x, int y) = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
};

