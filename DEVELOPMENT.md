Week 1:
- trouble generating the project but it should be working now
- still deciding on a game, either between Tetris or Conway's Game of Life
- thinking about how to represent pieces and their rotation in Tetris is going to be one of the hardest
  challenges in this project
  - 4/18: Pieces are implemented and they can do basic rotation and side-to-side movement. 
          I decided to hard-code piece rotations in a large array so as to not deal with rotation 
		  algorithms for such a small amount of pieces. As of right now, all of my work is being done 
		  on finishing the base Tetris game before I add GUI elements or game sound. I will need to 
		  make sure to finish the game ASAP because GUI and/or game sound will be the libraries 
		  fulfilling my library requirement.
  - 4/19: Pieces can fall into the right places for the most part, but there are a few cases where 
		  I need to check all the parts of a piece and whether they can fall, not just the lowest point.

Week 2:
- 4/20: Busy this weekend but I should be finishing the game within the next few days so I can get 
        started on GUI and sound libraries.
- 4/21: Today I worked on row clearing, which took a surprisingly long time to figure out
        both because of my coding decisions and needing to keep track of what updates what in my
		system. Imagine how much trouble multithreading is.
- 4/22: The entire base game of Tetris is almost done, with the major features left being a way to lose,
        colored pieces, wall kicks, and score. After that I will probably start working on the library
		requirement, using sound to play Tetris music and/or ofxDatGUI to make a nice interface. If there
		is still time afterwards, I will add piece queues and piece holds.
- 4/23: I am having an issue where if I register an ofxDatGuiLabel, none of my arrow keys work. I have 
        confirmed this by simply commenting out the one line where I register the label. Weird bug. Maybe 
		for now I'll just use drawBitMapString to display score instead.
  - Update: Apparently adding the label changed what key the computer thought I was pressing, 
            so instead of registering an arrow key press it thought it was some other random button. It 
			was a simple manual fix.
- 4/24: Ghost pieces have been added and color have been added. Ghost pieces are based on similar logic
        to the hard drop method, which makes sense because both methods need to know where the piece *would*
		fall. Lots of tweaks were needed to color to make sure I don't blind myself playing this game.
  - Update: Game sounds have been added!
- 4/25: The "next piece" indicator has been added, which was easy to implement because the technology
        to draw pieces already exists. The sound lag issues have been fixed. The mp3 file I was using 
		simply had a 0.2 second delay and I needed to cut it more.
- 4/29: Mostly all that's left in this game is extra features, like the ghost button toggle and volume
        slider I added today. The big things left that I want to do are making the game look aesthetically
		pleasing and refactoring some code.
- 5/2:  There's a really strange exception being thrown when I soft drop pieces, but the exception is thrown
        in OF and ofxDatGui code, none of which I understand nor want to modify. The program still exits with
		exit code 0. When I look at the call hierarchy, it tells me that my drawString call from ofTrueTypeFont
		is causing it, which makes little sense to me because the only thing it does is draw a string. Oh well.
		Note to self: Maybe just build the program in Release Mode.