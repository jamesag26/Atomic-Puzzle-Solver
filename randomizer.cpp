//  James Alford-Golojuch
//  Program 2
//  Randomizer code and moves from Program 1 are borrowed code, with some modifications, from Lydia Brothers from her post on the discussion board for week 4

#include "randomizer.hpp"

#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

int puzzleLength = 6;
//k moves from solved
//int k;
//0 is air
//1-21 ball values
//-1 unfillable space
vector<vector<int>> upper({{1,2,3,4,5,6},{7,8,9,10,11,-1},{12,13,14,15,-1,-1},{16,17,18,-1,-1,-1},{19,20,-1,-1,-1,-1},{21,-1,-1,-1,-1,-1}});
vector<vector<int>> lower({{0,0,0,0,0,0},{0,0,0,0,0,-1},{0,0,0,0,-1,-1},{0,0,0,-1,-1,-1},{0,0,-1,-1,-1,-1},{0,-1,-1,-1,-1,-1}});
vector<vector<vector<int>>> puzzleState;
bool ballDropLastMove = false;

//vectors that contain previous states
vector<vector<vector<int>>> prevLower;
vector<vector<vector<int>>> prevUpper;
//vector of previous moves
vector<char> prevMove;

//Holds initial solved states
vector<vector<int>> solvedLower = lower;
vector<vector<int>> solvedUpper = upper;

//r-red, g-green, y-yellow, b-blue, w-white, n-black
char availColors [6] = { 'r','g','y','b','w','n' };

char ballColor [22] = { '~',
    'r','r','r','r','r','r',
    'g','g','g','g','g',
    'y','y','y','y',
    'b','b','b',
    'w','w',
    'n'};

vector<vector<vector<int>>> shuffleBoard(int k){
    
    //Implementing Anton's method from the Discussion Board Week 2, with some minor adjustments by Evan and Olivia.
    //random legal move fcn
    //specific legal move fcn
    //f-flip, c-clockwise, k-counterclockwise
    
    prevUpper.push_back(upper);
    prevLower.push_back(upper);
    prevMove.push_back('f');
    //Step 1: make k legal random moves
    for(int i=0; i<k; i++){
        unknownLegalMove();
    }
    //Step 2: if in flat state, get out of flat state
    //Step 2a: to get out of flat state, if puzzle symmetrical, turn then flip, else flip
    if(isFlat()){
        if(isSymmetric()){
            givenLegalMove('c');
            givenLegalMove('f');
        }
        else{
            givenLegalMove('f');
        }
    }
    //Step 3: keep making random moves until flat state (max 200)
    //Step 3a: if over 200 moves, make turns until flat state
    //Step 3b: find closest symmetrical state
    
    int maxMoves = 200;
    int j=0;
    while(j<maxMoves && !isFlat()){
        unknownLegalMove();
        j += 1;
    }
    while(j >= maxMoves && !isFlat()){
        givenLegalMove('c');
    }
    int adjustUpper = (offsetUpper() - offsetLower() +6)%6;
    while(!isSymmetric()){
        if(adjustUpper < 3){
            givenLegalMove('k');
        }
        else{
            givenLegalMove('c');
        }
    }
    
    //Step 4: assign colors such that it's solved
    assignColors();
    //Step 5: load state k moves back
    loadState(prevUpper.size()-k-1);
    settleBoard();
    puzzleState.push_back(upper);
    puzzleState.push_back(lower);
    return puzzleState;
}

bool isFlat(){
    //checks bottom of upper array to see if there are any non-zero vals, if so it's not flat.
    for(int i=0; i<puzzleLength; i++){
        if(upper[i][0] != 0){
            return false;
        }
    }
    return true;
}

bool isSymmetric(){
    //checks to see if first column has same number of spaces for ball in top and bottom tube
    int lowBallNum = 0;
    int highBallNum = 0;
    for(int i=0; i<puzzleLength; i++){
        if(lower[0][i] != -1){
            lowBallNum += 1;
        }
        if(upper[0][i] != -1){
            highBallNum += 1;
        }
    }
    if(lowBallNum != highBallNum){
        return false;
    }
    return true;
}

void givenLegalMove(char move){
    if(move == 'f'){
        flip();
        prevUpper.push_back(upper);
        prevLower.push_back(lower);
        prevMove.push_back('f');
    }
    if(move == 'c'){
        clkRotate();
        prevUpper.push_back(upper);
        prevLower.push_back(lower);
        prevMove.push_back('c');
    }
    if(move == 'k'){
        cclkRotate();
        prevUpper.push_back(upper);
        prevLower.push_back(lower);
        prevMove.push_back('k');
        
    }
    //printBoard();
}

void flip(){
    //for each column
    //take elements in each column in upper, put in temp
    //put lower into upper
    //put temp into lower
    int temp;
    for(int j=0; j<puzzleLength; j++){
        for(int i=0; i<puzzleLength; i++){
            temp = upper[j][i];
            upper[j][i] = lower[j][i];
            lower[j][i] = temp;
        }
    }
    //if (gravity == 0){ gravity = 1; }
    //else{ gravity = 0; }
    //settleBoard(newUpper, newLower);
}

void clkRotate(){
    //for upper clkRotate
    int newTempCol[6] = {0,0,0,0,0,0};
    int oldTempCol[6] = {0,0,0,0,0,0};
    
    //seed oldTemp
    for(int j=0;j<6;j++){
        oldTempCol[j] = upper[0][j];
    }
    
    for(int i=0;i<6;(i++)){
        int newColIndex = (i+1)%6;
        
        //move new col to new temp
        for(int j=0;j<6;j++){
            newTempCol[j] = upper[newColIndex][j];
        }
        //move old temp to new col
        for(int j=0;j<6;j++){
            upper[newColIndex][j] =  oldTempCol[j];
        }
        //move new temp to old temp
        for(int j=0;j<6;j++){
            oldTempCol[j] = newTempCol[j];
        }
    }
    //settleBoard(newUpper, newLower);
}

void cclkRotate(){
    //if the value is modulable by the number of tubes then reset the number. Decrement if clockwise (move Right); Increment if counterclockwise (move left
    int newTempCol[6] = {0,0,0,0,0,0};
    int oldTempCol[6] = {0,0,0,0,0,0};
    
    //seed oldTemp
    for(int j=0;j<6;j++){
        oldTempCol[j] = upper[5][j];
    }
    
    for(int i=5;i>=0;(i--)){
        int newColIndex = (i+5)%6;
        
        //move new col to new temp
        for(int j=0;j<6;j++){
            newTempCol[j] = upper[newColIndex][j];
        }
        //move old temp to new col
        for(int j=0;j<6;j++){
            upper[newColIndex][j] =  oldTempCol[j];
        }
        //move new temp to old temp
        for(int j=0;j<6;j++){
            oldTempCol[j] = newTempCol[j];
        }
    }
    //settleBoard(newUpper, newLower);
}

void unknownLegalMove(){
    // Seed the RNG once, at the start of the program
    srand( time( NULL ));
    //illegal: flip and another flip
    //illegal: rotate then counter rotate (given no balls drop)
    vector<char> validMoves;
    if(prevMove[prevMove.size()-1] != 'f'){
        validMoves.push_back('f');
    }
    if(ballDropLastMove || (prevMove[prevMove.size()-1] != 'c')){
        validMoves.push_back('k');
    }
    if(ballDropLastMove || (prevMove[prevMove.size()-1] != 'k')){
        validMoves.push_back('c');
    }
    //srand(time(NULL));
    int chosenMove = rand() % validMoves.size();
    if(validMoves[chosenMove] == 'f'){
        flip();
        settleBoard();
        prevUpper.push_back(upper);
        prevLower.push_back(lower);
        prevMove.push_back('f');
    }
    if(validMoves[chosenMove] == 'c'){
        clkRotate();
        settleBoard();
        prevUpper.push_back(upper);
        prevLower.push_back(lower);
        prevMove.push_back('c');
    }
    if(validMoves[chosenMove] == 'k'){
        cclkRotate();
        settleBoard();
        prevUpper.push_back(upper);
        prevLower.push_back(lower);
        prevMove.push_back('k');
    }
    //printBoard();
}

void settleBoard(){
    //if it is not flipped and not settled, then we want to flip the non-negative portion of array and give it to other board
    
    //x is 0 to 12
    //y = x-6
    ballDropLastMove = false;
    for(int i=0; i<6; i++){
        for(int k=0; k<6; k++){
            for(int j=0; j<11; j++){
                int currVal = getValueAtPosition(i,j);
                int nextVal = getValueAtPosition(i,j+1);
                if(currVal == -1){
                    continue;
                }
                if(currVal == 0){
                    if(nextVal == 0 || nextVal == -1){
                        continue;
                    }
                    ballDropLastMove = true;
                    setValueAtPosition(i,j,nextVal);
                    setValueAtPosition(i,j+1,currVal);
                    
                }
            }
        }
    }
}

void setValueAtPosition(int i, int j, int input){
    //use this to rotate ball values into different tubes
    int row = j-6;
    if(row < 0){
        row = -1-row;
        lower[i][row] = input;
    }
    else{
        upper[i][row] = input;
    }
}

int getValueAtPosition(int i, int j){
    
    int row = j-6;
    if(row < 0){
        row = -1-row;
        return lower[i][row];
    }
    else{
        return upper[i][row];
    }
}

int offsetLower(){
    for(int i=0; i<puzzleLength; i++){
        if(lower[i][puzzleLength-1] != -1){ return i; }
    }
    //Returns in case of error
    return -1;
}

int offsetUpper(){
    for(int i=0; i<puzzleLength; i++){
        if(upper[i][puzzleLength-1] != -1){ return i; }
    }
    //Returns in case of error
    return -1;
}

void assignColors(){
    //assign colors to "solved" state
    if(!isFlat()){
        return;
    }
    for(int i = 0; i<puzzleLength; i++){
        for(int j = 0; j<puzzleLength; j++){
            int iOffset = (i+offsetLower())%6;
            //cout << j << "," << offsetLower() << "," << iOffset << endl;
            if(lower[iOffset][j] == -1){
                continue;
            }
            ballColor[lower[iOffset][j]] = availColors[i];
        }
    }
}

void loadState(int i){
    upper = prevUpper[i];
    lower = prevLower[i];
}