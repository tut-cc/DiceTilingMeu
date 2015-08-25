//石を置く盤面のクラス
#pragma once
#include "../../Stone/include/stone.h"
#include <memory>

class Field {
private:

public:
	static const int NO = 0;
	static const int ST = 1;
	virtual ~Field();
	virtual bool at(int x, int y) const = 0;
	virtual bool apply(Stone s, int x, int y) = 0;
	virtual std::unique_ptr<Field> clone() const = 0;
};

