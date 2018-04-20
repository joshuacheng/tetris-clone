#pragma once

#include "ofMain.h"
//#include "TetrisBoard.h"
#include "Piece.h"


const int BOX_SIZE = 50;
const int TETRIS_WIDTH = 10;
const int TETRIS_HEIGHT = 20;

class ofApp : public ofBaseApp {

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

	//TetrisBoard board_;
	bool board_[TETRIS_WIDTH][TETRIS_HEIGHT];

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

	Point piece_origin_;
	Point lowest_point_;
	int piece_type_;
	int piece_rotation_;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void drawPiece();
		void drawBoard();

		void makeNewPiece();
		void rotatePiece(Direction rotation);
		void softDrop();
		void hardDrop();
		void horizontalMove(Direction direction);
		
};
