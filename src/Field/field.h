//石を置く盤面のクラス
#include "../Stone/stone.h"

#define ST 1
#define NO 0
class Field {
private:
	int[][] field;
	
public:
	//問題と同様の形式の文字列が渡される
	Field(string str){
	}
	
	//標準入力からFieldを作成する
	Field(){
	}
	virtual bool at(int x, int y) = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
};

