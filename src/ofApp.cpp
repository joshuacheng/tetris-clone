#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	piece_origin.x = 4;
	piece_origin.y = 0;
	piece_ = 2;
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
void ofApp::keyPressed(int key){

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
	//ofSetLineWidth(1);

	ofFill();

	for (int i = 0; i < 4; i++) {
		int boardX = piece_origin.x + pointRotations_[piece_][0][i].x;
		int boardY = piece_origin.y + pointRotations_[piece_][0][i].y;
		board_[boardX][boardY] = true;
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

