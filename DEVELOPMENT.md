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
        