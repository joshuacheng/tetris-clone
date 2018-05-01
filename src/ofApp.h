#pragma once

#include "ofMain.h"
//#include "Piece.h"
#include "ofxDatGui.h"

const uint64_t PIECE_DELAY = 700000000;

const int BOX_SIZE = 50;
const int TETRIS_WIDTH = 10;
const int TETRIS_HEIGHT = 21;

// Where to draw the tetris board.
const int TETRIS_START_X = 100;
const int TETRIS_START_Y = 50;

const int NEXT_BOX_X = 700;
const int NEXT_BOX_Y = 150;
const int HOLD_BOX_X = 700;
const int HOLD_BOX_Y = 700;

const int ROW_MULTIPLIER = 100;

class ofApp : public ofBaseApp, public ofThread {
	
	enum GameState {
		IN_PROGRESS,
		PAUSED,
		GAME_OVER
	};

	enum Direction {
		LEFT,
		RIGHT
	};

	/* 
	    The order of these piece types matches the order
	    they appear in the array.
	*/
	enum PieceType {
		I, J, L, O, S, T, Z
	};

	/*
		Simple structure that describes point
		with (x, y)
	*/
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

	// TODO: probably change to color array
	ofColor board_[TETRIS_WIDTH][TETRIS_HEIGHT];

	/* 
	    pointRotations[piece_type][number of rotations][points in specific rotation]
		Each piece's points are oriented with respect to an imaginary 3x3 or 4x4 square
		around the piece, where the top left corner is (0,0).
		See https://vignette.wikia.nocookie.net/tetrisconcept/images/3/3d/SRS-pieces.png/revision/latest?cb=20060626173148
		for visualization.

		TODO: Maybe refactor into separate classes later 
		      which use std::array to get list of points
		
	*/
	Point pointRotations_[7][4][4] = {
		// I
		{
			{Point(0, 1),Point(1, 1),Point(2, 1),Point(3, 1) },
			{Point(1, 0),Point(1, 1),Point(1, 2),Point(1, 3) },
			{Point(0, 2),Point(1, 2),Point(2, 2),Point(3, 2) },
			{Point(2, 0),Point(2, 1),Point(2, 2),Point(2, 3) }
		},

		// J
		{
			{Point(0, 0),Point(0, 1),Point(1, 1),Point(2, 1) },
			{Point(1, 0),Point(2, 0),Point(1, 1),Point(1, 2) },
			{Point(0, 1),Point(1, 1),Point(2, 1),Point(2, 2) },
			{Point(1, 0),Point(1, 1),Point(1, 2),Point(0, 2) }
		},

		// L
		{
			{Point(0, 1),Point(1, 1),Point(2, 1),Point(2, 0) },
			{Point(1, 0),Point(1, 1),Point(1, 2),Point(2, 2) },
			{Point(0, 1),Point(1, 1),Point(2, 1),Point(0, 2) },
			{Point(1, 0),Point(1, 1),Point(1, 2),Point(0, 0) }
		},

		// O
		{
			{Point(0, 0),Point(0, 1),Point(1, 0),Point(1, 1) },
			{Point(0, 0),Point(0, 1),Point(1, 0),Point(1, 1) },
			{Point(0, 0),Point(0, 1),Point(1, 0),Point(1, 1) },
			{Point(0, 0),Point(0, 1),Point(1, 0),Point(1, 1) }
		},

		// S
		{
			{Point(1, 0),Point(2, 0),Point(0, 1),Point(1, 1) },
			{Point(1, 0),Point(1, 1),Point(2, 1),Point(2, 2) },
			{Point(0, 2),Point(1, 2),Point(1, 1),Point(2, 1) },
			{Point(0, 0),Point(0, 1),Point(1, 1),Point(1, 2) }
		},

		// T
		{
			{Point(1, 0),Point(0, 1),Point(1, 1),Point(2, 1) },
			{Point(1, 0),Point(1, 1),Point(1, 2),Point(2, 1) },
			{Point(0, 1),Point(1, 1),Point(2, 1),Point(1, 2) },
			{Point(1, 0),Point(1, 1),Point(0, 1),Point(1, 2) }
		},

		// Z
		{
			{Point(0, 0),Point(1, 0),Point(1, 1),Point(2, 1) },
			{Point(2, 0),Point(2, 1),Point(1, 1),Point(1, 2) },
			{Point(0, 1),Point(1, 1),Point(1, 2),Point(2, 2) },
			{Point(1, 0),Point(0, 1),Point(1, 1),Point(0, 2) }
		}
	};

	ofColor colors[7] = { ofColor::red,
						ofColor::magenta,
						ofColor::yellow,
						ofColor::cyan,
						ofColor::blue,
						ofColor::green,
						ofColor::teal };

	Point piece_origin_;

	/*
		Lowest point of the current block with respect
		to the board's coordinates, not piece_origin_
	*/
	Point lowest_point_;
	int piece_type_;
	int piece_rotation_;

	int next_piece_;
	int hold_piece_;
	bool already_swapped;

	int player_score_;
	bool show_ghosts_;

	GameState game_state_;
	ofTimer timer_;

	ofTrueTypeFont tetris_font_;
	ofTrueTypeFont score_font_;

	ofSoundPlayer game_music_;
	ofSoundPlayer move_effect_;
	ofSoundPlayer rotate_effect_;
	ofSoundPlayer line_clear_;

	ofxDatGui *gui_;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
	
		// ---- Draw ----
		void drawPiece();
		void drawGhostPiece();
		void drawBoard();
		void drawScore();
		void drawNextPiece();
		void drawHoldPiece();
		void drawGameOver();

		// ---- Piece manipulation ----
		void makeNewPiece();
		void swapHoldPiece();
		void rotatePiece(Direction rotation);
		void softDrop();
		void hardDrop();
		bool horizontalMove(Direction direction);
		void clearRows();
		bool rowIsEmpty(int row);
		bool updateLowestPoint();
		void setPiecesToBoard();
		void reset();

		// ---- GUI methods ----
		void onToggleEvent(ofxDatGuiToggleEvent e);
		void onSliderEvent(ofxDatGuiSliderEvent e);

		// ---- Colors ----
		bool isColorDefault(const ofColor &color);

		// ---- Setup ----
		void setUpMusic();
		void setUpSoundEffects();
		void setUpGui();

		// ---- Thread for auto piece drop ----

		// Scheduler used for making piece automatically fall.
		// Idea from http://openframeworks.cc/documentation/utils/ofTimer/
		void threadedFunction();
};
