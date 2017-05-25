//  James Alford-Golojuch
//  Program 2
//  Heuristic is based upon Anton Vinogradov's post about his heuristic from the discussion board for week 4

#include "heuristic.hpp"
#include <vector>

using namespace std;

//Size of puzzle
int puzzleLength2 = 6;

//r-red, g-green, y-yellow, b-blue, w-white, n-black
char hAvailColors [6] = { 'r','g','y','b','w','n' };

char hBallColor [22] = { '~',
    'r','r','r','r','r','r',
    'g','g','g','g','g',
    'y','y','y','y',
    'b','b','b',
    'w','w',
    'n'};

int heuristicValue(vector<vector<int>> upper, vector<vector<int>> lower){
    int h = 0;
    int offset;
    //Finds distance tubes are offset from each other since solution requires offset of 0
    if (hOffsetUpper(upper) > hOffsetLower(lower)){ offset = hOffsetUpper(upper) - hOffsetLower(lower); }
    else{ offset = hOffsetLower(lower) - hOffsetUpper(upper); }
    
    if (offset < 4){ h = offset; }
    //After offset is equivalent to 3 any further increases actually represents a decrease in distance due to turning in circle
    else if (offset == 4){ h = offset - 2; }
    else if (offset == 5){ h = offset - 4; }
    //This is an error
    else { return h = -1; }
    
    //If puzzle isnt in flat state then at least one turn will be needed to
    if (!hIsFlat(upper)) { h++;}
    //If there are balls misplaced in bottom tubes puzzle has to be at least one step away
    if (!hIsSolved(lower) && h < 1){ h = 1; }
    return h;
}

bool hIsFlat(vector<vector<int>> upper){
    //checks bottom of upper array to see if there are any non-zero vals, if so it's not flat.
    for(int i=0; i<puzzleLength2; i++){
        if(upper[i][0] != 0){
            return false;
        }
    }
    return true;
}

bool hIsSolved(vector<vector<int>> lower){
    //if (!isSymmetric()) { return false; }
    for(int i = 0; i<puzzleLength2; i++){
        for(int j = 0; j<puzzleLength2; j++){
            int iOffset = (i+hOffsetLower(lower))%6;
            if(lower[iOffset][j] == -1){ continue; }
            if(hBallColor[lower[iOffset][j]] != hAvailColors[i]){ return false; }
        }
    }
    return true;
}

int hOffsetLower(vector<vector<int>> lower){
    for(int i=0; i<puzzleLength2; i++){
        if(lower[i][puzzleLength2-1] != -1){ return i; }
    }
    //Returns in case of error
    return -1;
}

int hOffsetUpper(vector<vector<int>> upper){
    for(int i=0; i<puzzleLength2; i++){
        if(upper[i][puzzleLength2-1] != -1){ return i; }
    }
    //Returns in case of error
    return -1;
}