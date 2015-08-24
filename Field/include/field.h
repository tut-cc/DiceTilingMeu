//石を置く盤面のクラス
#pragma once
#include "../Stone/stone.h"
#include <memory>

class Field {
private:
	
public:
	static const int NO = 0;
	static const int ST = 1;
	//問題と同様の形式の文字列が渡される
	Field(string str){
	}
	
	//標準入力からFieldを作成する
	Field(){
	}
	virtual bool at(int x, int y) = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
	std::unique_ptr<Field> clone() = 0;
};

