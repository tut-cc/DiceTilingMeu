#include "ExtendedField.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

const int ExtendedField::tx[] = { 1, 0, -1, 0 };
const int ExtendedField::ty[] = { 0, -1, 0, 1 };

ExtendedField::ExtendedField(std::vector<std::string>) {

}
ExtendedField::ExtendedField(const bool mat[32][32], const decltype(history) &) {}

//FieldをExtendedFieldに変換するコンストラクタ
ExtendedField::ExtendedField(Field &f) {

}

bool ExtendedField::at(int x, int y) const{
	return true;
}
bool ExtendedField::appliable(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) const {
	return false;
}
void ExtendedField::apply(std::shared_ptr<Stone> s, int x, int y, int reverse, int angle) {

}
std::unique_ptr<Field> ExtendedField::clone() const {
	return nullptr;
}
std::unique_ptr<ExtendedField> ExtendedField::clone_ex() const {
	return nullptr;
}

int ExtendedField::eval_final_score() {
	return block_count;
}
int ExtendedField::eval_select_score() {
	return 0;
}