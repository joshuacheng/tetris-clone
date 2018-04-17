#pragma once
#include <vector>

struct Point {
	int x, y;
	Point() {
		x = y = 0;
	};
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

enum PIECE_TYPE {
	I,
	O,
	T,
	S,
	Z,
	L,
	J
};

class Piece {

	//Point pointRotations_[4][4];
	std::vector<Point> pointRotations_;
	
public:

	explicit Piece(PIECE_TYPE piece_type);


};