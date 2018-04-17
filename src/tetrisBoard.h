#pragma once

const int TETRIS_WIDTH = 10;
const int TETRIS_HEIGHT = 20;

class TetrisBoard {

	bool board_[TETRIS_WIDTH][TETRIS_HEIGHT];

public:

	TetrisBoard() : board_{ 0 } {};

	bool spotIsFilled(int x, int y) const;
	


};