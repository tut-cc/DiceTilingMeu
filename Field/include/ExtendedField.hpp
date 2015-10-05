#pragma once
#include <vector>
#include <string>
#include <array>

#include "field.hpp"
#include "ExtendedStone.hpp"
#include "HistoryTree.hpp"

class ExtendedField : public Field {
private:
	//�t�B�[���h���r�b�g��ŕ\��
	RowBit bitmat[32];

	//���Ƀu���b�N���u����Ȃ���΂Ȃ�Ȃ��ʒu��\��
	RowBit next_block[32];

	//�t�B�[���h�ɑ��݂���u���b�N��
	int block_count = 0;

	//�t�B�[���h�̍ő�󂫃}�X��
	static int space_count;
	//
	bool mat[32][32];
	bool ok[32][32];

	int value = -1;

	virtual bool appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const override;
	virtual void apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) override;

public:
	//4���������邽�߂̔z��

	int parent_idx = 0;

	ExtendedField(std::vector<std::string>);
	ExtendedField(const bool mat[32][32], const decltype(history) &);
	ExtendedField(const ExtendedField &f, const decltype(history) &);
	ExtendedField(const ExtendedField &f);

	virtual ~ExtendedField() = default;
	virtual bool at(int x, int y) const override;
	virtual bool at_ok(int x, int y) const;

	virtual bool appliable_ex(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) const;
	virtual void apply_ex(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle);

	virtual bool appliable_bit(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle) const;
	virtual void apply_bit(std::shared_ptr<ExtendedStone> s, int x, int y, int reverse, int angle);

	virtual std::unique_ptr<Field> clone() const;
	virtual std::shared_ptr<ExtendedField> clone_ex() const;

	int eval_final_score();
	int eval_select_score();

	friend std::ostream& operator << (std::ostream& os, const std::shared_ptr<ExtendedField>& p);

	std::string get_bit_str();
	std::string get_neighbor_str();

	void set_bit(int x, int y);
	int get_bit(int x, int y);
};
