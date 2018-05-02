#include "ofApp.h"
#include <iostream>

/*
	TODO: 
	1. Colors [DONE]
	1.5. FIX SCORE LABEL MAYBE JUST USE TRUETYPEFONT [DONE]
	2. Ghost pieces [DONE]
	2.5 Toggle ghost pieces [LATER]
	3. Wall kicks [DONE]
	4. Game sound [DONE]
	4.25 ADD NEXT PIECE [DONE]
	4.5 ADD PIECE HOLDS [NEXT]
	5. Make background black i guess
	6. do a bunch of refactoring
	7. better UI

*/

//--------------------------------------------------------------
void ofApp::setup() {
	player_score_ = 0;
	show_ghosts_ = true;
	play_sound_effects_ = true;

	tetris_font_.load("goodtime.ttf", 60);
	score_font_.load("tetris_block.ttf", 30);
	controls_font_.load("Verdana.ttf", 30);

	setUpMusic();
	setUpSoundEffects();
	setUpGui();

	next_piece_ = rand() % 7;
	hold_piece_ = -1;

	makeNewPiece();
	game_state_ = IN_PROGRESS;

	timer_.setPeriodicEvent(PIECE_DELAY);
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
		timer_.waitNext();
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
	if (game_state_ == PAUSED) {
		ofSetColor(ofColor::black);
		tetris_font_.drawString("PAUSED", TETRIS_START_X + 50, TETRIS_START_Y + 500);
	}
	else if (game_state_ == GAME_OVER) {
		drawGameOver();
		return;
	}

	drawControls();

	drawBoard();
	drawPiece();

	drawNextPiece();
	drawHoldPiece();
	drawScore();
}

void ofApp::keyPressed(int key) {
	int key_upper = toupper(key);

	if (game_state_ == GAME_OVER && key_upper == 'R') {
		reset();
	}

	// Pause and unpause
	if (key_upper == 'P') {
		if (game_state_ == IN_PROGRESS) {
			game_state_ = PAUSED;
			game_music_.setPaused(true);
		}
		else if (game_state_ == PAUSED) {
			game_state_ = IN_PROGRESS;
			game_music_.setPaused(false);
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
	else if (key_upper == 'C' && game_state_ == IN_PROGRESS) {
		swapHoldPiece();
	}

}

// -------------------- Draw methods ----------------------

/*
    The int piece defines which Tetris piece is drawn from 
	the pointRotations array.
	Ex: 0 refers to the I-piece.

*/
void ofApp::drawPiece() {
	
	for (int i = 0; i < 4; i++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][i].x;
		int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][i].y;

		// Hide part of piece if above screen.
		if (boardY == 0) {
			continue;
		}

		// Draw in the piece.
		ofSetColor(colors[piece_type_]);
		ofFill();
		ofDrawRectangle(TETRIS_START_X + boardX * BOX_SIZE + 1, TETRIS_START_Y + boardY * BOX_SIZE + 1, BOX_SIZE - 1, BOX_SIZE - 1);

		// Draw the piece border.
		ofSetColor(ofColor::black);
		ofNoFill();
		ofDrawRectangle(TETRIS_START_X + boardX * BOX_SIZE + 2, TETRIS_START_Y + boardY * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
		
		if (show_ghosts_) {
			drawGhostPiece();
		}
	}
}

/*
	Draws the projection of where your piece will fall.
*/
void ofApp::drawGhostPiece() {
	bool canDrop = true;
	int peeksAhead = 0;

	ofSetLineWidth(10);

	while (canDrop) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
			int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y + peeksAhead;

			// Once we've found the farthest we can drop down..
			if (boardY >= TETRIS_HEIGHT - 1 || !isColorDefault(board_[boardX][boardY + 1])) {
				ofSetColor(ofColor::darkBlue);
				
				// Draw the ghost piece.
				for (int part = 0; part < 4; part++) {
					int x = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][part].x;
					int y = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][part].y + peeksAhead;
					ofDrawRectangle(TETRIS_START_X + x * BOX_SIZE, TETRIS_START_Y + y * BOX_SIZE, BOX_SIZE, BOX_SIZE);
				}

				// Clean up line width and get out.
				ofSetLineWidth(2);
				return;
			}
		}
		peeksAhead++;
	}

}

void ofApp::drawBoard() {
	ofSetLineWidth(2);
	ofSetColor(ofColor::black);
	ofNoFill();


	for (int i = 0; i < TETRIS_WIDTH; ++i) {
		for (int j = 1; j < TETRIS_HEIGHT; ++j) {

			// Draw board with color
			if (!isColorDefault(board_[i][j])) {
				ofSetColor(board_[i][j]);
				ofFill();
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 1, TETRIS_START_Y + j * BOX_SIZE + 1, BOX_SIZE - 1, BOX_SIZE - 1);
				ofSetColor(ofColor::black);
				ofNoFill();
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 2, TETRIS_START_Y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
			}
			else {
				ofDrawRectangle(TETRIS_START_X + i * BOX_SIZE + 2, TETRIS_START_Y + j * BOX_SIZE + 2, BOX_SIZE - 2, BOX_SIZE - 2);
			}

		}
	}
}

void ofApp::drawScore() {
	ofColor score_color = colors[piece_type_];
	score_color.setBrightness(192);
	ofSetColor(score_color);
	score_font_.drawString("SCORE " + std::to_string(player_score_), 700, 1100);
}

void ofApp::drawNextPiece() {
	score_font_.drawString("NEXT", 700, 100);
	ofSetColor(ofColor::black);
	ofNoFill();
	ofDrawRectangle(NEXT_BOX_X, NEXT_BOX_Y, 300, 300);

	Point start(NEXT_BOX_X + 50, NEXT_BOX_Y + 50);

	for (int part = 0; part < 4; part++) {
		int xDraw = start.x + pointRotations_[next_piece_][0][part].x * BOX_SIZE;
		int yDraw = start.y + pointRotations_[next_piece_][0][part].y * BOX_SIZE;

		// Draw in the piece.
		ofSetColor(colors[next_piece_]);
		ofFill();
		ofDrawRectangle(xDraw + 1, yDraw + 1, BOX_SIZE - 1, BOX_SIZE - 1);

		// Draw the piece border.
		ofSetColor(ofColor::black);
		ofNoFill();
		ofDrawRectangle(xDraw + 2, yDraw + 2, BOX_SIZE - 2, BOX_SIZE - 2);

	}
}

void ofApp::drawHoldPiece() {
	score_font_.drawString("HOLD", 700, 600);
	ofSetColor(ofColor::black);
	ofNoFill();
	ofDrawRectangle(HOLD_BOX_X, HOLD_BOX_Y, 300, 300);

	Point start(HOLD_BOX_X + 50, HOLD_BOX_Y + 50);
	
	if (hold_piece_ >= 0 && hold_piece_ < 7) {
		for (int part = 0; part < 4; part++) {
			int xDraw = start.x + pointRotations_[hold_piece_][0][part].x * BOX_SIZE;
			int yDraw = start.y + pointRotations_[hold_piece_][0][part].y * BOX_SIZE;

			// Draw in the piece.
			ofSetColor(colors[hold_piece_]);
			ofFill();
			ofDrawRectangle(xDraw + 1, yDraw + 1, BOX_SIZE - 1, BOX_SIZE - 1);

			// Draw the piece border.
			ofSetColor(ofColor::black);
			ofNoFill();
			ofDrawRectangle(xDraw + 2, yDraw + 2, BOX_SIZE - 2, BOX_SIZE - 2);

		}
	}
}

void ofApp::drawGameOver() {
	ofSetColor(ofColor::black);
	tetris_font_.load("goodtime.ttf", 60);
	tetris_font_.drawString("GAME OVER", TETRIS_START_X - 50, TETRIS_START_Y + 500);
	tetris_font_.load("goodtime.ttf", 30);
	tetris_font_.drawString("Press R to restart game", TETRIS_START_X - 50, TETRIS_START_Y + 600);
}

void ofApp::drawControls() {
	ofSetColor(ofColor::black);
	controls_font_.drawString("Controls", 1400, 300);
	ofDrawRectangle(1250, 320, 650, 600);
	controls_font_.drawString("Left/Right: Move piece", 1300, 400);
	controls_font_.drawString("Z/X: Rotate piece left/right", 1300, 500);
	controls_font_.drawString("C: Hold piece", 1300, 600);
	controls_font_.drawString("P: Pause", 1300, 700);
	controls_font_.drawString("Space: Hard drop", 1300, 800);
	
}
// ------------- Piece manipulation --------------

void ofApp::makeNewPiece() {
	piece_origin_.x = 4;
	piece_origin_.y = 0;
	lowest_point_.x = 0;
	lowest_point_.y = 0;

	piece_type_ = next_piece_;
	next_piece_ = rand() % 7;
	already_swapped = false;
	piece_rotation_ = 0;

	draw();

	// Check if the piece cannot move upon creation and trigger game loss.
	bool canMove = updateLowestPoint();

	if (!canMove) {
		game_state_ = GAME_OVER;
		stopThread();
		game_music_.stop();
		game_music_.load("tetris_gameover.mp3");
		game_music_.setLoop(false);
		game_music_.play();
	}
}

void ofApp::swapHoldPiece() {

	/* 
		-1 represents nothing in hold, special case
		because nothing gets swapped out
	*/
	if (hold_piece_ == -1) {
		hold_piece_ = piece_type_;
		makeNewPiece();
	}
	else if (hold_piece_ >= 0 && hold_piece_ < 7 && !already_swapped) {
		// Swap hold and current piece
		int temp_piece = hold_piece_;
		hold_piece_ = piece_type_;
		piece_type_ = temp_piece;
		already_swapped = true;
	}
}

/*
	Rotates a piece based on the rotation enum.
	Wall kicks if piece is up against a wall.
*/
void ofApp::rotatePiece(Direction rotation) {
	int new_rotation;

	if (rotation == RIGHT) {
		new_rotation = (piece_rotation_ + 1) % 4;
	}
	else if (rotation == LEFT) {
		new_rotation = ((piece_rotation_ - 1) % 4 + 4) % 4;
	}

	for (int piecePart = 0; piecePart < 4; piecePart++) {
		int boardX = piece_origin_.x + pointRotations_[piece_type_][new_rotation][piecePart].x;
		int boardY = piece_origin_.y + pointRotations_[piece_type_][new_rotation][piecePart].y;

		// If the piece would go off screen, try moving right, left
		// and rotating first. (This is a basic wall kick.)
		if (boardX >= TETRIS_WIDTH || boardX < 0 || boardY >= TETRIS_HEIGHT 
			|| !isColorDefault(board_[boardX][boardY])) {
			
			if (horizontalMove(RIGHT)) {
				rotatePiece(rotation);
			}
			if (horizontalMove(LEFT)) {
				rotatePiece(rotation);
			}
			return;
		}
	}
	piece_rotation_ = new_rotation;
	if (play_sound_effects_) {
		rotate_effect_.play();
	}
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
		if (y_spot_below >= TETRIS_HEIGHT || !isColorDefault(board_[boardX][y_spot_below])) {
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
			
			if (boardY >= TETRIS_HEIGHT - 1 || !isColorDefault(board_[boardX][boardY + 1])) {
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
	Returns true if move was successful, otherwise false.
*/
bool ofApp::horizontalMove(Direction direction) {	

	if (direction == RIGHT) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
			int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y;

			// If the piece moving right would go off screen or hit a piece, don't.
			if (boardX + 1 >= TETRIS_WIDTH || !isColorDefault(board_[boardX + 1][boardY])) {
				return false;
			}
		}
		piece_origin_.x++;
	}
	else if (direction == LEFT) {
		for (int piecePart = 0; piecePart < 4; piecePart++) {
			int boardX = piece_origin_.x + pointRotations_[piece_type_][piece_rotation_][piecePart].x;
			int boardY = piece_origin_.y + pointRotations_[piece_type_][piece_rotation_][piecePart].y;

			if (boardX - 1 < 0 || !isColorDefault(board_[boardX - 1][boardY])) {
				return false;
			}
		}
		piece_origin_.x--;
	}

	if (play_sound_effects_) {
		move_effect_.play();
	}

	return true;
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

			if (isColorDefault(board_[col][rowToCheck])) {
				skipRow = true;
				break;
			}
		}

		if (skipRow) {
			continue;
		}

		// If entire row was there, clear it.
		for (int col = 0; col < TETRIS_WIDTH; col++) {
			board_[col][rowToCheck].set(ofColor());
		}

		rowsCleared++;
	}

	player_score_ += ROW_MULTIPLIER * rowsCleared;
	if (rowsCleared > 0 && play_sound_effects_) {
		line_clear_.play();
	}

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
			board_[col][rowToCheck] = board_[col][peekRow];
			board_[col][peekRow].set(ofColor());
		}
	}

}

/*
	Returns true if an entire row is false. Otherwise, return false.
*/
bool ofApp::rowIsEmpty(int row) {
	for (int col = 0; col < TETRIS_WIDTH; col++) {
		if (!isColorDefault(board_[col][row])) {
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
		if (!isColorDefault(board_[boardX][boardY + 1])) {
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
		board_[boardX][boardY] = colors[piece_type_];
	}
}

/*
	Reset and restart the game.
*/
void ofApp::reset() {

	// Clear the board.
	for (int row = 0; row < TETRIS_HEIGHT; row++) {
		for (int col = 0; col < TETRIS_WIDTH; col++) {
			board_[col][row].set(ofColor());
		}
	}

	setup();
}

// -------------- GUI -------------------
void ofApp::onToggleEvent(ofxDatGuiToggleEvent e) {
	if (e.target->getLabel() == "Show ghost pieces") {
		show_ghosts_ = !show_ghosts_;
	}
	else if (e.target->getLabel() == "Play sound effects") {
		play_sound_effects_ = !play_sound_effects_;
	}
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
	double raw_volume = e.target->getValue();

	// Map raw_volume to value between 0 and 1 for ofSoundPlayer
	float mapped_volume = ofMap(raw_volume, 0, 100, 0, 1);
	game_music_.setVolume(mapped_volume);
}

// --------------- Colors ----------------

bool ofApp::isColorDefault(const ofColor &color) {
	return (color.r == 255 && color.g == 255
		&& color.b == 255 && color.a == 255);
}


// --------------- Setup -----------------

void ofApp::setUpMusic() {
	game_music_.load("tetris_music.mp3");
	game_music_.setLoop(true);
	game_music_.setVolume(0.1);
	game_music_.play();
}

void ofApp::setUpSoundEffects() {
	move_effect_.load("move_sound.mp3");
	move_effect_.setMultiPlay(true);
	rotate_effect_.load("rotate_sound.mp3");
	rotate_effect_.setMultiPlay(true);
	line_clear_.load("line_clear.mp3");
	line_clear_.setMultiPlay(true);
}

void ofApp::setUpGui() {
	gui_ = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
	gui_->addToggle("Show ghost pieces", true);
	gui_->getToggle("Show ghost pieces")->setWidth(700);
	gui_->addToggle("Play sound effects", true);
	gui_->getToggle("Play sound effects")->setWidth(700);
	gui_->onToggleEvent(this, &ofApp::onToggleEvent);
	gui_->addSlider("Volume", 0, 100, 10);
	gui_->getSlider("Volume")->setWidth(700, 200);
	gui_->getSlider("Volume")->setPrecision(0);
	gui_->onSliderEvent(this, &ofApp::onSliderEvent);

}
