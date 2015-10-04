#pragma once
#include "ExtendedField.hpp"
#include "ExtendedStone.hpp"

typedef std::tuple<int, int, int, int> PlaceTuple;


//置ける場所を記憶しておくクラス
class PlaceLists {
private:
	//r, a, x, y
	std::vector<std::vector<PlaceTuple>> appliable_list;

public:
	PlaceLists(std::shared_ptr<ExtendedField> field, std::vector<std::shared_ptr<ExtendedStone>> stones);

	std::vector<PlaceTuple> get_list(int stone_id);
};

PlaceLists::PlaceLists(std::shared_ptr<ExtendedField> field, std::vector<std::shared_ptr<ExtendedStone>> stones)
{
	for (int i = 0; i < stones.size(); i++) {
		auto pl = stones[i]->get_place_list();

		std::vector<PlaceTuple> es_tuple;
		for (auto p : pl) {
			int r = p.first;
			int a = p.second;
			for (int y = -7; y < 32; y++) {
				for (int x = -7; x < 32; x++) {

					if (field->appliable_bit(stones[i], x, y, r, a)) {
						es_tuple.emplace_back(r, a, x, y);
					}
				}
			}
		}
		appliable_list.push_back(es_tuple);
	}
}

inline std::vector<PlaceTuple> PlaceLists::get_list(int stone_id)
{
	return appliable_list[stone_id];
}
