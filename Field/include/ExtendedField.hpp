#pragma once
#include "field.hpp"
#include  "ExtendedStone.hpp"
#include <vector>
#include <string>
#include <array>

class ExtendedField : public Field {
private:
	//フィールドをビット列で表す
	__int32 bitmat[32];

	//次にブロックが置かれなければならない位置を表す
	__int32 next_block[32];

	//4方向を見るための配列
	static const int tx[];
	static const int ty[];

	//フィールドに存在するブロック数
	int block_count = 0;

	//
	bool mat[32][32];
	bool ok[32][32];

	int value = -1;

	virtual bool appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const override;
	virtual void apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) override;

public:
	ExtendedField(std::vector<std::string>);
	ExtendedField(const bool mat[32][32], const decltype(history) &);
	ExtendedField(Field &f);

	virtual ~ExtendedField() = default;
	virtual bool at(int x, int y) const override;

	virtual bool appliable_ex(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) const;
	virtual void apply_ex(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle);

	virtual std::unique_ptr<Field> clone() const;
	virtual std::shared_ptr<ExtendedField> clone_ex() const;

	int eval_final_score();
	int eval_select_score();

	friend std::ostream& operator << (std::ostream& os, const ExtendedField& p);
	friend std::ostream& operator << (std::ostream& os, const std::shared_ptr<ExtendedField>& p);
};
