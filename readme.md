9 puzzle project
====================

### Puzzle ###
* A 9 puzzle is a puzzle where there is 8 tiles and a blank space.
The user's goal is to end up with the puzzle in a specific configuration

* This program allows the user to attempt to complete a 9 puzzle by clicking on the files that they wish to move.

* This project also has a help function that uses the A* algorithm to find the simplest path to goal and then gives the user a hint as to what tile they should click on next.

### Algoritm ###
* The algorithm that is used in the help functionality of this program is the A* algoritm.

* The solver checks if the current configuration is the goal configuration.

* If it is not, the solver generates all of the possibilities that the player could make and generates scores for each configuration based on the number of moves that have been made so far and the hamming distance.

* The hamming distance is the number of cells excluding the blank cell that are not in their correct place.

* The solver then gets the root of the bin heap since this is the configuration with the lowest score and then recurs on itself with this new configuration.

* If the config is at the goal, the solver returns to the main function with the goal configuration containing the number of moves and the path it took to get there.

### Controls ###

* **Left Click** - Moves a cell into the blank cell
* **Right Click** - Gives the player a hint

* **Win Condition** - The player reaches the goal configuration, numbers in order from top left to bottom right and the space in the middle.

### Installing this project ###

* The user must have X11 installed for mac and linux I use XQuartz for mac

#### Build Commands ####
* **Windows** - g++ -o lab3.exe lab3.cpp -O2 -lgdi32
* **Linux** - g++ -o lab3 lab3.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 
* **Mac** - g++ -Wall -o main main.cpp -I/usr/X11R6/include -L/usr/X11R6/lib -lX11

* Test