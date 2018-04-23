#include "ofApp.h"
#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
	makeNewPiece();

	timer = new Scheduler(&piece_origin_);
}

//--------------------------------------------------------------
void ofApp::update() {
	// Reset lowest point.
	lowest_point_.x = 0;
	lowest_point_.y = 0;

	bool canDrop = updateLowestPoint();

	// Check if piece hit floor or another piece
	if (lowest_point_.y == TETRIS_HEIGHT - 1 || !canDrop) {

		// Set the board tiles on piece to true for drawing purposes.
		setPiecesToBoard();
		makeNewPiece();
	}

	//std::cout << lowest_point_.y << "/" << piece_type_ << std::endl;

}

//--------------------------------------------------------------
void ofApp::draw(){

	drawBoard();
	drawPiece();
}

//--------------------------------------------------------------
// TODO: don't let player hold down to keep rotating piece
// reference solution: https://forum.openframeworks.cc/t/multiple-keys-and-key-related-questions/2034/2
// TODO: implement ghost

void ofApp::keyPressed(int key){
	int key_upper = toupper(key);

	if (key_upper == 'X' || key_upper == OF_KEY_UP) {
		rotatePiece(RIGHT);
	}
	else if (key_upper == 'Z') {
		rotatePiece(LEFT);
	}
	else if (key_upper == OF_KEY_RIGHT) {
		horizontalMove(RIGHT);
	}
	else if (key_upper == OF_KEY_LEFT) {
		horizontalMove(LEFT);
	}
	else if (key_upper == OF_KEY_DOWN) {
		softDrop();
	}
	else if (key_upper == ' ') { // ' ' is space bar
		hardDrop();
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

/*
    The int piece defines which Tetris piece is drawn from the pointRotations array.
	Ex: 0 refers to the I-piece.

*/
void ofApp::drawPiece() {
	
	ofSetColor(ofColor::blue);
	ofFill();

	for (int i = 0; i < 4; i++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][i].x;
		int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][i].y;
		//board_[boardX][boardY] = true;
		ofDrawRectangle(100 + boardX * BOX_SIZE + 2, 100 + boardY * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
	}
}



void ofApp::drawBoard() {
	ofSetLineWidth(1);
	ofSetColor(ofColor::black);
	ofNoFill();
	float start_x = 100;
	float start_y = 100;


	for (int i = 0; i < TETRIS_WIDTH; ++i) {
		for (int j = 0; j < TETRIS_HEIGHT; ++j) {

			// Draw pieces already filled in blue for now.
			// TODO: save colors somehow maybe color array
			if (board_[i][j]) {
				ofSetColor(ofColor::blue);
				ofFill();
				ofDrawRectangle(start_x + i * BOX_SIZE + 2, start_y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
				ofSetColor(ofColor::black);
				ofNoFill();
			}
			else {
				ofDrawRectangle(start_x + i * BOX_SIZE + 2, start_y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
			}
		}
	}
}

void ofApp::makeNewPiece() {
	piece_origin_.x = 4;
	piece_origin_.y = 0;
	lowest_point_.x = 0;
	lowest_point_.y = 0;

	int random_piece = rand() % 6;
	piece_type_ = random_piece;
	piece_rotation_ = 0;
}

/*
	Rotates a piece based on the rotation enum.
*/

void ofApp::rotatePiece(Direction rotation) {
	int new_rotation;

	if (rotation == RIGHT) {
		new_rotation = (piece_rotation_ + 1) % 4;
	}
	else if (rotation == LEFT) {
		new_rotation = ((piece_rotation_ - 1) % 4 + 4) % 4;
	}
	// TODO: implement kick instead of do nothing
	for (int piecePart = 0; piecePart < 4; piecePart++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][new_rotation][piecePart].x;
		int boardY = piece_origin_.y + pointRotations_[piece_type_][new_rotation][piecePart].y;

		// If the piece would go off screen, don't.
		if (boardX >= TETRIS_WIDTH || boardX < 0 || boardY >= TETRIS_HEIGHT) {
			return;
		}
	}
	piece_rotation_ = new_rotation;
}

/* 
    A soft drop is when the player wants to move their piece down 
    one tile at a time faster than the regular timer
*/
void ofApp::softDrop() {
	for (int piecePart = 0; piecePart < 4; piecePart++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
		int y_spot_below = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y + 1;

		// If the piece moving down would go off screen or hit another block, don't.
		if (y_spot_below >= TETRIS_HEIGHT || board_[boardX][y_spot_below]) {
			return;
		}
	}

	piece_origin_.y++;
	updateLowestPoint();
	clearRows();
}

/*
	A hard drop is when player wants to drop to lowest possible point
	in current column.
*/
void ofApp::hardDrop() {
	int spacesDropped = 0;
	int canDrop = true;

	int peeksAhead = 0;

	while (canDrop) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
			int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y + peeksAhead;
			
			if (boardY >= TETRIS_HEIGHT - 1 || board_[boardX][boardY + 1]) {
				piece_origin_.y += spacesDropped;
				setPiecesToBoard();
				updateLowestPoint();
				clearRows();
				makeNewPiece();
				return;
			}
		}
		peeksAhead++;
		spacesDropped++;
	}
	
}

/*
	Helper function for moving the piece left or right. 
*/
void ofApp::horizontalMove(Direction direction) {	

	if (direction == RIGHT) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
			int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y;

			// If the piece moving right would go off screen or hit a piece, don't.
			if (boardX + 1 >= TETRIS_WIDTH || board_[boardX + 1][boardY]) {
				return;
			}
		}
		piece_origin_.x++;
	}
	else if (direction == LEFT) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
			int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y;

			if (boardX - 1 < 0 || board_[boardX - 1][boardY]) {
				return;
			}
		}
		piece_origin_.x--;
	}

}

/*
	This method is called whenever a piece falls into place, NOT on every update.
	pretty self-explanatory, clears the rows possible and drops above pieces down
*/

void ofApp::clearRows() {

	// Clear up to 4 rows.
	for (int rowToCheck = lowest_point_.y, i = 0; i < 4; rowToCheck--, i++) {
		bool skipRow = false;

		// Make sure the whole row is there.
		for (int col = 0; col < TETRIS_WIDTH; col++) {

			if (!board_[col][rowToCheck]) {
				skipRow = true;
				break;
			}
		}

		if (skipRow) {
			continue;
		}

		// If entire row was there, clear it.
		for (int col = 0; col < TETRIS_WIDTH; col++) {
			board_[col][rowToCheck] = false;
		}
	}
	 
	// Push above rows down to appropriate spot.
	// TODO: IMPLEMENT
	for (int rowToCheck = lowest_point_.y; rowToCheck > 0; rowToCheck--) {
		if (!rowIsEmpty(rowToCheck)) {
			continue;
		}

		// Check which row to pull down to ground.
		int peekRow = rowToCheck;
		while (rowIsEmpty(peekRow)) {
			if (peekRow == 0) {
				return;
			}
			peekRow--;
		}

		// "Drop" the row down to the ground.
		for (int col = 0; col < TETRIS_WIDTH; col++) {
			board_[col][rowToCheck] = board_[col][peekRow];
			board_[col][peekRow] = false;
		}
	}
	
}

/*
	Returns true if an entire row is false. Otherwise, return false.
*/
bool ofApp::rowIsEmpty(int row) {
	for (int col = 0; col < TETRIS_WIDTH; col++) {
		if (board_[col][row]) {
			return false;
		}
	}

	return true;
}

/*
	This method returns whether the piece can drop to the next column after update.
*/
bool ofApp::updateLowestPoint() {
	bool canDrop = true;

	// Constantly update the piece's lowest point.
	for (int piecePart = 0; piecePart < 4; piecePart++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
		int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y;
		if (lowest_point_.y < boardY) {
			lowest_point_.x = boardX;
			lowest_point_.y = boardY;
		}

		// If any part of the current piece is obstructed one spot below:
		if (board_[boardX][boardY + 1]) {
			canDrop = false;
		}
	}

	return canDrop;
}

/*
	Makes current piece become a part of the board.
*/
void ofApp::setPiecesToBoard() {
	for (int piecePart = 0; piecePart < 4; piecePart++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
		int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y;
		board_[boardX][boardY] = true;
	}
}
