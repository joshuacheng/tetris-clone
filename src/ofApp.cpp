#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	piece_origin_.x = 4;
	piece_origin_.y = 0;
	piece_ = 4;
	piece_rotation_ = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){


	drawBoard();
	drawPiece();
	
}

//--------------------------------------------------------------
// TODO: don't let player hold down to keep rotating piece
// reference solution: https://forum.openframeworks.cc/t/multiple-keys-and-key-related-questions/2034/2
void ofApp::keyPressed(int key){
	int key_upper = toupper(key);

	if (key_upper == 'X' || key_upper == OF_KEY_UP) {
		rotatePiece(RIGHT);
	}
	else if (key_upper == 'Z') {
		rotatePiece(LEFT);
	}
	else if (key_upper == OF_KEY_RIGHT) {
		move(RIGHT);
	}
	else if (key_upper == OF_KEY_LEFT) {
		move(LEFT);
	}
	else if (key_upper == OF_KEY_DOWN) {
		softDrop();
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
		int boardX = piece_origin_.x + pointRotations_[piece_][piece_rotation_][i].x;
		int boardY = piece_origin_.y + pointRotations_[piece_][piece_rotation_][i].y;
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
			ofDrawRectangle(start_x + i * BOX_SIZE, start_y + j * BOX_SIZE, BOX_SIZE, BOX_SIZE);
		}
	}
}

/*
	Rotates a piece based on the rotation enum.
*/

// TODO: implement wall checking and kicks
void ofApp::rotatePiece(DIRECTION rotation) {
	if (rotation == RIGHT) {
		piece_rotation_ = (piece_rotation_ + 1) % 4;
	}
	else {
		// Extra modulo calculation required for potential negative number.
		piece_rotation_ = ((piece_rotation_ - 1) % 4 + 4) % 4;
	}
}

/* 
    A soft drop is when the player wants to move their piece down 
    one tile at a time faster than the regular timer
*/
void ofApp::softDrop() {
	for (int piecePart = 0; piecePart < 4; piecePart++) {
		int boardY = piece_origin_.y + pointRotations_[piece_][piece_rotation_][piecePart].y + 1;

		// If the piece moving right would go off screen, don't.
		if (boardY >= TETRIS_HEIGHT) {
			return;
		}
	}

	piece_origin_.y++;
}

/*
	move is the helper function for moving the piece left or right. 
*/
void ofApp::move(DIRECTION direction) {	

	if (direction == RIGHT) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_][piece_rotation_][piecePart].x + 1;
			
			// If the piece moving right would go off screen, don't.
			if (boardX >= TETRIS_WIDTH) {
				return;
			}
		}
		piece_origin_.x++;
	}
	else if (direction == LEFT) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_][piece_rotation_][piecePart].x - 1;

			// If the piece moving right would go off screen, don't.
			if (boardX < 0) {
				return;
			}
		}
		piece_origin_.x--;
	}

}

