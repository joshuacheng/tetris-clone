#pragma once
#include <vector>


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

	// Point pointRotations_[4][4];
	// std::vector<Point> pointRotations_;
	
public:

	explicit Piece(PIECE_TYPE piece_type);


};