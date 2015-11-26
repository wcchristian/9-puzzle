//
//  main.cpp
//  Project2
//
//  This program generates a 9 puzzle that the user can solve by left-clicking on tiles.
//  The user can right click to get a hint.
//  The goal configuration is as follows:
//  1 2 3
//  4   5
//  6 7 8
//
// Build and run command(for my computer) to include CImg and X11:
// g++ -Wall -o main main.cpp -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 && ./main
//
//  Created by Christian Andersen on 11/8/15.
//

//-----------------------------IMPORTS-----------------------------------------
#include "BinHeap.h"
#include "CImg.h"

#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;
using namespace cimg_library;

//--------------------------------UTILITIES--------------------------------------
//This function loads a puzzle array in from a text file. See data1.txt for an example as to how these are laid out.
int **loadPuzzle(int x, int y, int fileNumber)
{
    //create the array to be returned.
    int **puzzle = new int*[y];
    for(int i=0; i<y; i++) {
        puzzle[i] = new int[x];
    }
    //Take the random number and create the file name to open
    ostringstream stream;
    stream << "data" << fileNumber << ".txt";
    string filename = stream.str();
    ifstream infile(filename.c_str());

    //If file doesn't exist.
    if (!infile)
        errorMsg("File doesn't exist",true);

    //Insert the data into the array.
    for(int row=0; row<y; row++) {
        for(int col=0; col<x; col++) {
            infile >> puzzle[row][col];
        }
        cout << endl;
    }

    return puzzle;
}

//DEBUG: Dumps the array to the screen.
void dumpArray(string msg, int **c, int x, int y)
{
    if (msg.length()>0)
        cout<<msg<<endl;

    for (int i=0;i<y;i++) {
        for(int j=0; j<x; j++) {
            cout << c[i][j];
        }
        cout << endl;
    }
}

//--------------------------------CIMG utilities--------------------------------------
//Draws a grid to the canvas. This provides the outline for the puzzle.
void drawGrid(CImg<unsigned char> &canvas, int cellWidth, int cellHeight, int yMargin, int xMargin, int numberOfXCells, int numberOfYCells, int cHeight, int cWidth) {
    unsigned char white[] = {128,128,128};

    for(int i=0; i<=numberOfYCells; i++) {
        canvas.draw_line(xMargin, (yMargin + i*cellHeight), (xMargin + cellWidth*numberOfYCells), (yMargin + i*cellHeight), white);
    }

    for(int i=0; i<=numberOfXCells; i++) {
        canvas.draw_line((xMargin + i*cellWidth), yMargin, (xMargin + i*cellWidth), (yMargin+cellHeight*numberOfXCells), white);
    }
}

//Uses the current array to copy image files included in this directory to the canvas.
//The switch statement takes the 2d array into account as it chooses an image to overlay. 1-8 are images black-8, 10 is hint, and 999 displays the
//win tile.
void copyImg(int **array, int xMargin, int yMargin, int cellWidth, int cellHeight, CImg<unsigned char> &canvas, CImg<unsigned char> black,
             CImg<unsigned char> img1,
             CImg<unsigned char> img2,
             CImg<unsigned char> img3,
             CImg<unsigned char> img4,
             CImg<unsigned char> img5,
             CImg<unsigned char> img6,
             CImg<unsigned char> img7,
             CImg<unsigned char> img8,
             CImg<unsigned char> win,
             CImg<unsigned char> hint,
             int x, int y) {
    for(int i=0; i<y; i++) {
        for(int j=0; j<x; j++) {
            for(int a=1; a<cellWidth; a++) {
                for(int b=1; b<cellHeight; b++) {
                    switch(array[j][i]) {
                        case 0:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)black(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)black(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)black(a, b, 2);
                            break;
                        case 1:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img1(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img1(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img1(a, b, 2);
                            break;
                        case 2:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img2(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img2(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img2(a, b, 2);
                            break;
                        case 3:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img3(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img3(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img3(a, b, 2);
                            break;
                        case 4:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img4(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img4(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img4(a, b, 2);
                            break;
                        case 5:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img5(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img5(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img5(a, b, 2);
                            break;
                        case 6:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img6(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img6(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img6(a, b, 2);
                            break;
                        case 7:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img7(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img7(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img7(a, b, 2);
                            break;
                        case 8:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)img8(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)img8(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)img8(a, b, 2);
                            break;
                        case 10:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)hint(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)hint(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)hint(a, b, 2);
                            break;
                        case 999:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)win(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)win(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)win(a, b, 2);
                            break;
                        default:
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 0) = (unsigned char)black(a, b, 0);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 1) = (unsigned char)black(a, b, 1);
                            canvas(xMargin+i*cellWidth+a, yMargin+j*cellHeight+b, 2) = (unsigned char)black(a, b, 2);
                    }

                }
            }
        }
    }
}

//--------------------------------Solver Utils--------------------------------------

//This function populates the binary heap with the possible places that the blank tile can travel at its given location.
//The heap then will perc up/down to ensure that the possible config with the lowest overall score will sit at the root of
//the tree.
void populateHeap(BinHeap &bh, Config c) {
    //Get the zero position
    int posX = 0;
    int posY = 0;

    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(c.getArray()[i][j] == 0) {
                posY = i;
                posX = j;
            }
        }
    }

    if(posY == 0 || posY == 1) { // can move down
        if(c.getPath().back() != 'u') {
            int **a = c.getArray();

            a[posY][posX] = a[posY+1][posX];
            a[posY+1][posX] = 0;

            Config nc(a, c.getNumberOfMoves()+1, c.getPath()+"d");
            bh.insert(nc);
        }
    }
    if(posX == 1 || posX ==2) { //can move left
        if(c.getPath().back() != 'r') {
            int **a = c.getArray();

            a[posY][posX] = a[posY][posX-1];
            a[posY][posX-1] = 0;

            Config nc(a, c.getNumberOfMoves()+1, c.getPath()+"l");
            bh.insert(nc);
        }
    }
    if(posY == 1 || posY == 2) { //can move up
        if(c.getPath().back() != 'd') {
            int **a = c.getArray();

            a[posY][posX] = a[posY-1][posX];
            a[posY-1][posX] = 0;

            Config nc(a, c.getNumberOfMoves()+1, c.getPath()+"u");
            bh.insert(nc);
        }
    }
    if(posX == 0 || posX == 1) { //can move right
        if(c.getPath().back() != 'l') {
            int **a = c.getArray();

            a[posY][posX] = a[posY][posX+1];
            a[posY][posX+1] = 0;

            Config nc(a, c.getNumberOfMoves()+1, c.getPath()+"r");
            bh.insert(nc);
        }
    }
}

//This is the recursive algorithm that solves the puzzle.
void solvePuzzle(Config &c, Config goal, BinHeap &bh) {
    //generate possibilities
    populateHeap(bh, c);

    //if not at goal by the time the path is 20 or more, return.
    //This solves the issue of a puzzle that cannot be solved.
    if(c.getPath().length() >= 20) {
        return;
    }
    //if not at goal configuration call this function with the min
    if(!c.equalConfig(goal)) {
        c = bh.getMin();
        solvePuzzle(c, goal, bh);
    }
}

//-----------------------------------------MAIN-------------------------------------------------------
int main(int argc, const char * argv[]) {

    //Create goal configuration
    Config goal;

    //Create/read-in starting configuration
       srand(static_cast<int>(time(0))); // This seeds the rand function so that it returns different values.
       int **a;
       int r = (rand() % 6)+1;
       a = loadPuzzle(3, 3, r);

    //display the starting configuration on the screen.
       int xCells=3;
	   int yCells=3;
	   int yMargin = 15;
	   int xMargin = 50;
	   
	   //setup canvas.
	   CImg<unsigned char> img1("img1.bmp");
	   CImg<unsigned char> img2("img2.bmp");
	   CImg<unsigned char> img3("img3.bmp");
	   CImg<unsigned char> img4("img4.bmp");
	   CImg<unsigned char> img5("img5.bmp");
	   CImg<unsigned char> img6("img6.bmp");
	   CImg<unsigned char> img7("img7.bmp");
	   CImg<unsigned char> img8("img8.bmp");
	   CImg<unsigned char> black("black.bmp");
       CImg<unsigned char> win("win.bmp");
       CImg<unsigned char> hint("hint.bmp");
	   
	   int cWidth = img1.width()*xCells+(xMargin*2);
	   int cHeight = img1.height()*yCells+(yMargin*2);
	   
	   CImg <unsigned char> canvas(cWidth,cHeight,1,3,0);
	   CImgDisplay disp(canvas,"9 Puzzle Game");

       bool done = false;

       Config current(a, 0, "");
       BinHeap bh(0);

    //while the window is not closed.
	while(!disp.is_closed()) {
        //load configuration on the screen
	   drawGrid(canvas, img1.width(), img1.height(), yMargin, xMargin, xCells, yCells, cHeight, cWidth);
	   disp.display(canvas);
	   copyImg(a, xMargin, yMargin, img1.width(), img1.height(), canvas, black, img1, img2, img3, img4, img5, img6, img7, img8, win, hint, xCells, yCells);

       //If the puzzle is not solved
       if(!done) {
        //If the user left clicks
           if(disp.button()&1) {
                int x = disp.mouse_x();
                int y = disp.mouse_y();
                int xLoc=-1;
                int yLoc=-1;
                
                //Get the cell location of the user clicked cell.
                if(x > xMargin && y > yMargin && x < (xMargin+cWidth*xCells) && y < (yMargin+cHeight*yCells)) {
                    x -= xMargin;
                    y -= yMargin;
                    while(x>0) {
                        xLoc++;
                        x -= img1.width();
                    }
                    
                    while(y>0) {
                        yLoc++;
                        y -= img1.height();
                }

                //If the click is in a bordering cell to the empty cell
                //Swap the cells
                if(yLoc+1 <= yCells-1 && yLoc+1 >= 0) { //down
                    if(a[yLoc+1][xLoc] == 0) {
                        a[yLoc+1][xLoc] = a[yLoc][xLoc];
                        a[yLoc][xLoc] = 0;

                    }
                }
                if(xLoc-1 <= xCells-1 && xLoc-1 >= 0) { //left
                    if(a[yLoc][xLoc-1] == 0) {
                        a[yLoc][xLoc-1] = a[yLoc][xLoc];
                        a[yLoc][xLoc] = 0;
                    }
                }
                if(yLoc-1 <= yCells-1 && yLoc-1 >= 0) { //up
                    if(a[yLoc-1][xLoc] == 0) {
                        a[yLoc-1][xLoc] = a[yLoc][xLoc];
                        a[yLoc][xLoc] = 0;
                    }
                } 
                if(xLoc+1 <= xCells+1 && xLoc+1 >=0) { //right
                    if(a[yLoc][xLoc+1] == 0) {
                        a[yLoc][xLoc+1] = a[yLoc][xLoc];
                        a[yLoc][xLoc] = 0;
                    }
                }
            }
            //update current with the current config array.
            current = Config(a, 0, "");
            //Display the goal state if the current configuration is the goal.
            if(current.equalConfig(goal)) {
                a[1][1] = 999;
                done = true;
            }
            //wait for user input.
            disp.wait();
           }
           //If the user right clicks
           if(disp.button()&2) {
                int xLoc;
                int yLoc;
                //get location of the blank cell.
                for(int i=0; i<3; i++) {
                    for(int j=0; j<3; j++) {
                        if(a[i][j] == 0) {
                            xLoc = j;
                            yLoc = i;
                        }
                    }
                }
                //Solve the puzzle
                solvePuzzle(current, goal, bh);

                int tmp;
                //Flash the hint cell for the direction that the next move in the solution is.
                if(current.getPath().at(0) == 'd') {
                    tmp = a[yLoc+1][xLoc];
                    a[yLoc+1][xLoc] = 10;
                    copyImg(a, xMargin, yMargin, img1.width(), img1.height(), canvas, black, img1, img2, img3, img4, img5, img6, img7, img8, win, hint, xCells, yCells);
                    disp.display(canvas);
                    a[yLoc+1][xLoc] = tmp;
                } else if (current.getPath().at(0) == 'r') {
                    tmp = a[yLoc][xLoc+1];
                    a[yLoc][xLoc+1] = 10;
                    copyImg(a, xMargin, yMargin, img1.width(), img1.height(), canvas, black, img1, img2, img3, img4, img5, img6, img7, img8, win, hint, xCells, yCells);
                    disp.display(canvas);
                    a[yLoc][xLoc+1] = tmp;
                } else if(current.getPath().at(0) == 'u') {
                    tmp = a[yLoc-1][xLoc];
                    a[yLoc-1][xLoc] = 10;
                    copyImg(a, xMargin, yMargin, img1.width(), img1.height(), canvas, black, img1, img2, img3, img4, img5, img6, img7, img8, win, hint, xCells, yCells);
                    disp.display(canvas);
                    a[yLoc-1][xLoc] = tmp;
                } else if(current.getPath().at(0) == 'l') {
                    tmp = a[yLoc][xLoc-1];
                    a[yLoc][xLoc-1] = 10;
                    copyImg(a, xMargin, yMargin, img1.width(), img1.height(), canvas, black, img1, img2, img3, img4, img5, img6, img7, img8, win, hint, xCells, yCells);
                    disp.display(canvas);
                    a[yLoc][xLoc-1] = tmp;
                }
                //Sleep so that the user has a chance to see the hint.
                this_thread::sleep_for(chrono::seconds(1));
           }
        }
	}
    return 0;
}
