//
//  Config.h
//  Project2
//  The config class houses the configurations for the puzzle
//
//  Created by Christian Andersen on 11/18/15.
//

#ifndef Config_h
#define Config_h

//includes
#include <iostream>

//namespaces
using namespace std;

//class
class Config {
public:
    Config(); //Creates a config with the goal state
    Config(int **a, int moves, string path);
    
    void dumpConfig();
    int getNumberOfMoves();
    string getPath();
    int** getArray();
    int getScore();
    int getHammingDistance(int **c);
    bool equalConfig(Config c);
    
private:
    string path;
    int moves;
    int hamming;
    int score;
    int ary[3][3];
};

//Constructors
Config::Config() {
    ary[0][0] = 1;
    ary[0][1] = 2;
    ary[0][2] = 3;
    ary[1][0] = 4;
    ary[1][1] = 0;
    ary[1][2] = 5;
    ary[2][0] = 6;
    ary[2][1] = 7;
    ary[2][2] = 8;
    moves = 0;
    path = "";
}

Config::Config(int **a, int m, string p) {
    ary[0][0] = a[0][0];
    ary[0][1] = a[0][1];
    ary[0][2] = a[0][2];
    ary[1][0] = a[1][0];
    ary[1][1] = a[1][1];
    ary[1][2] = a[1][2];
    ary[2][0] = a[2][0];
    ary[2][1] = a[2][1];
    ary[2][2] = a[2][2];
    moves = m;

    //convert to pointer
    int **b;
    b = new int*[3];

    for (int i = 0; i < 3; ++i) {
        b[i] = new int[3];
    }

    for (int i = 0; i < 3; ++i) {   // for each row
        for (int j = 0; j < 3; ++j) { // for each column
            b[i][j] = ary[i][j];
        }
    }
    hamming = getHammingDistance(b);
    score = hamming + moves;
    path = p;
}

//Other functions
void Config::dumpConfig() {
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            cout << ary[i][j] << " ";
        }
        cout << endl;
    }
}

int Config::getNumberOfMoves() {
    return moves;
}

string Config::getPath() {
    return path;
}

int** Config::getArray() {
    int **a;
    a = new int*[3];

    for (int i = 0; i < 3; ++i) {
        a[i] = new int[3];
    }

    for (int i = 0; i < 3; ++i) {   // for each row
        for (int j = 0; j < 3; ++j) { // for each column
            a[i][j] = ary[i][j];
        }
    }

    return a;
}

int Config::getScore() {
    return score;
}

//Gets the hamming distance, which is the number of cells that are out of place in the puzzle.
int Config::getHammingDistance(int **a) {
    //Loop through each row and column and increment the result at each  position where the tile is not in place.
    int result = 0;
    Config c;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(ary[i][j] != 0) {
                if (a[i][j] != c.getArray()[i][j]) {
                    result++;
                }
            }
        }
    }

    return result;
}

bool Config::equalConfig(Config c) {
    bool flag = false;

    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(ary[i][j] != c.getArray()[i][j]) {
                flag = true;
            }
        }
    }

    return !flag;
}

#endif /* Config_h */
