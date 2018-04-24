#include "ofApp.h"
#include <iostream>

/*
	TODO: 
	1. Colors [DONE]
	1.5. FIX SCORE LABEL MAYBE JUST USE BITMAPSTRING
	2. Ghost pieces (+ toggling them)
	3. Wall kicks
	4. Game sound
	5. do a bunch of refactoring
	6. better UI

*/

//--------------------------------------------------------------
void ofApp::setup(){
	player_score_ = 0;
	score_label_ = new ofxDatGuiLabel("SCORE: " + std::to_string(player_score_));
	score_label_->setPosition(1000, 1000);
	score_label_->setTheme(new ofxDatGuiThemeSmoke());

	tetrisFont.load("goodtime.ttf", 60);

	makeNewPiece();
	game_state_ = IN_PROGRESS;

	timer.setPeriodicEvent(700000000);
	startThread();
	
}

//--------------------------------------------------------------

/*
	Because Tetris is a tick-based game that does not need continuous
	updates, my "updating" all happens inside threadedFunction().
*/
void ofApp::update() {
}

void ofApp::threadedFunction() {
	while (isThreadRunning()) {
		timer.waitNext();
		if (game_state_ == IN_PROGRESS) {
			lowest_point_.x = 0;
			lowest_point_.y = 0;

			bool canDrop = updateLowestPoint();

			// Check if piece hit floor or another piece
			if (lowest_point_.y == TETRIS_HEIGHT - 1 || !canDrop) {

				// Set the board tiles on piece to true for drawing purposes.
				setPiecesToBoard();
				clearRows();
				makeNewPiece();
			}

			piece_origin_.y++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	drawBoard();
	drawPiece();

	// FIX THIS LINE
	//score_label_->setLabel("SCORE: " + std::to_string(player_score_));
	//score_label_->draw();
	
	if (game_state_ == PAUSED) {
		ofSetColor(ofColor::black);
		tetrisFont.drawString("PAUSED", TETRIS_START_X + 50, TETRIS_START_Y + 500);
	}
	else if (game_state_ == GAME_OVER) {
		ofSetColor(ofColor::black);
		tetrisFont.drawString("GAME OVER", TETRIS_START_X - 50, TETRIS_START_Y + 500);
		//tetrisFont.load("goodtime.ttf", 30);
		tetrisFont.drawString("Press R to restart game", TETRIS_START_X - 50, TETRIS_START_Y + 600);
	}
}

//--------------------------------------------------------------
// TODO: don't let player hold down to keep rotating piece
// reference solution: https://forum.openframeworks.cc/t/multiple-keys-and-key-related-questions/2034/2
// TODO: implement ghost

void ofApp::keyPressed(int key){
	int key_upper = toupper(key);

	if (game_state_ == GAME_OVER && key_upper == 'R') {
		reset();
	}

	// Pause and unpause
	if (key_upper == 'P') {
		if (game_state_ == IN_PROGRESS) {
			game_state_ = PAUSED;
		}
		else if (game_state_ == PAUSED) {
			game_state_ = IN_PROGRESS;
		}
		return;
	}

	if ((key_upper == 'X' || key == OF_KEY_UP) && game_state_ == IN_PROGRESS) {
		rotatePiece(RIGHT);
	}
	else if (key_upper == 'Z' && game_state_ == IN_PROGRESS) {
		rotatePiece(LEFT);
	}
	else if (key == OF_KEY_RIGHT && game_state_ == IN_PROGRESS) {
		horizontalMove(RIGHT);
	}
	else if (key == OF_KEY_LEFT && game_state_ == IN_PROGRESS) {
		horizontalMove(LEFT);
	}
	else if (key == OF_KEY_DOWN && game_state_ == IN_PROGRESS) {
		softDrop();
	}
	else if (key_upper == ' '  && game_state_ == IN_PROGRESS) { // ' ' is space bar
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
	
	ofSetColor(colors[piece_type_]);
	ofFill();

	for (int i = 0; i < 4; i++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][i].x;
		int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][i].y;
		
		// Hide part of piece if above screen.
		if (boardY == 0) {
			continue;
		}
		ofDrawRectangle(TETRIS_START_X + boardX * BOX_SIZE + 2, TETRIS_START_Y + boardY * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
	}
}



void ofApp::drawBoard() {
	ofSetLineWidth(1);
	ofSetColor(ofColor::black);
	ofNoFill();


	for (int i = 0; i < TETRIS_WIDTH; ++i) {
		for (int j = 1; j < TETRIS_HEIGHT; ++j) {

			// Draw board with color
			if (!isColorDefault(board1_[i][j])) {
				ofSetColor(board1_[i][j]);
				ofFill();
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 1, TETRIS_START_Y + j * BOX_SIZE + 1, BOX_SIZE - 1, BOX_SIZE - 1);
				ofSetColor(ofColor::black);
				ofNoFill();
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 2, TETRIS_START_Y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
			}
			else {
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 2, TETRIS_START_Y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
			}

			// Draw pieces already filled in blue for now.
			// TODO: save colors somehow maybe color array
			/*
			if (board_[i][j]) {
				ofSetColor(ofColor::blue);
				ofFill();
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 2, TETRIS_START_Y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
				ofSetColor(ofColor::black);
				ofNoFill();
			}
			else {
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 2, TETRIS_START_Y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
			}
			*/
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

	draw();

	// Check if the piece cannot move upon creation and trigger game loss.
	bool canMove = updateLowestPoint();

	if (!canMove) {
		std::cout << "LOSE" << std::endl;
		game_state_ = GAME_OVER;
	}
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
		/*
		// If the piece moving down would go off screen or hit another block, don't.
		if (y_spot_below >= TETRIS_HEIGHT || board_[boardX][y_spot_below]) {
			return;
		}
		*/
		if (y_spot_below >= TETRIS_HEIGHT || !isColorDefault(board1_[boardX][y_spot_below])) {
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
			
			if (boardY >= TETRIS_HEIGHT - 1 || !isColorDefault(board1_[boardX][boardY + 1])) {
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
			if (boardX + 1 >= TETRIS_WIDTH || !isColorDefault(board1_[boardX + 1][boardY])) {
				return;
			}
		}
		piece_origin_.x++;
	}
	else if (direction == LEFT) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
			int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y;

			if (boardX - 1 < 0 || !isColorDefault(board1_[boardX - 1][boardY])) {
				return;
			}
		}
		piece_origin_.x--;
	}

}

/*
	This method is called whenever a piece falls into place, NOT on every update.
	pretty self-explanatory, clears the rows possible and drops above pieces down.
	Also awards score to the player.
*/

void ofApp::clearRows() {
	int rowsCleared = 0;

	// Clear up to 4 rows.
	for (int rowToCheck = lowest_point_.y, i = 0; i < 4; rowToCheck--, i++) {
		bool skipRow = false;

		// Make sure the whole row is there.
		for (int col = 0; col < TETRIS_WIDTH; col++) {

			if (isColorDefault(board1_[col][rowToCheck])) {
				skipRow = true;
				break;
			}
		}

		if (skipRow) {
			continue;
		}

		// If entire row was there, clear it.
		for (int col = 0; col < TETRIS_WIDTH; col++) {
			//board_[col][rowToCheck] = false;
			board1_[col][rowToCheck].set(ofColor());
		}

		rowsCleared++;
	}

	player_score_ += ROW_MULTIPLIER * rowsCleared;
	 
	// Push above rows down to appropriate spot.
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
			//board_[col][rowToCheck] = board_[col][peekRow];
			//board_[col][peekRow] = false;
			board1_[col][rowToCheck] = board1_[col][peekRow];
			board1_[col][peekRow].set(ofColor());
		}
	}

}

/*
	Returns true if an entire row is false. Otherwise, return false.
*/
bool ofApp::rowIsEmpty(int row) {
	for (int col = 0; col < TETRIS_WIDTH; col++) {
		if (!isColorDefault(board1_[col][row])) {
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
		if (!isColorDefault(board1_[boardX][boardY + 1])) {
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
		//board_[boardX][boardY] = true;
		board1_[boardX][boardY] = colors[piece_type_];
	}
}

/*
	Reset and restart the game.
*/
void ofApp::reset() {

	// Clear the board.
	for (int row = 0; row < TETRIS_HEIGHT; row++) {
		for (int col = 0; col < TETRIS_WIDTH; col++) {
			//board_[col][row] = false;
			board1_[col][row].set(ofColor());
		}
	}

	game_state_ = IN_PROGRESS;
	makeNewPiece();
}

// --------------- Colors ----------------

bool ofApp::isColorDefault(const ofColor &color) {
	return (color.r == 255 && color.g == 255
		&& color.b == 255 && color.a == 255);
}