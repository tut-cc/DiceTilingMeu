#pragma once
#include "field.hpp"
#include <vector>
#include <string>
#include <array>

class ExtendedField : public Field {
private:
	//�t�B�[���h���r�b�g��ŕ\��
	__int32 bitmat[32];

	//���Ƀu���b�N���u����Ȃ���΂Ȃ�Ȃ��ʒu��\��
	__int32 next_block[32];

	//4���������邽�߂̔z��
	static const int tx[];
	static const int ty[];

	//�t�B�[���h�ɑ��݂���u���b�N��
	int block_count = 0;

	//

public:
	ExtendedField(std::vector<std::string>);
	ExtendedField(const bool mat[32][32], const decltype(history) &);
	ExtendedField(Field &f);
	virtual ~ExtendedField() = default;
	virtual bool at(int x, int y) const override;
	virtual bool appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const override;
	virtual void apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) override;
	virtual std::unique_ptr<Field> clone() const;
	virtual std::unique_ptr<ExtendedField> clone_ex() const;

	int eval_final_score();
	int eval_select_score();
};

#pragma once
