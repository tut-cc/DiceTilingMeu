#include "PlaceInfo.hpp"

PlaceInfo::PlaceInfo(){}

/*
stone_id	8bit	15 - 22	00000000 0IDIDIDI D0000000 00000000
x			6bit	9  - 14	00000000 00000000 0XXXXXX0 00000000
y			6bit	3  -  8	00000000 00000000 0000000Y YYYYY000
reverse		1bit	2  -  2	00000000 00000000 00000000 00000R00
angle		2bit	0  -  1	00000000 00000000 00000000 000000AA

*/
PlaceInfo::PlaceInfo(int stone_id, int x, int y, int reverse, int angle) {
	value = (stone_id << 15) | ((x+8) << 9) | ((y+8) << 3) | (reverse << 2) | (angle);

}

int PlaceInfo::get_id() { return (value >> 15) & 0xFF; }
int PlaceInfo::get_x() { return ((value >> 9) & 0x3F) - 8; }
int PlaceInfo::get_y() { return ((value >> 3) & 0x3F) - 8; }
int PlaceInfo::get_reverse() { return (value >> 2) & 0x01; }
int PlaceInfo::get_angle() { return value & 0x03; }

std::string PlaceInfo::get_str()
{
//	std::cout << "#" << value << std::endl;
	return (boost::format("%d %d %c %d\n") % get_x() % get_y() % (get_reverse() ? 'T' : 'H') % (get_angle() * 90)).str();
}
