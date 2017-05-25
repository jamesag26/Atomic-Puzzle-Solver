//
//  nextStateMoves.cpp
//  AIProgram2
//
//  Created by Jason  Sands on 9/16/16.
//  Copyright © 2016 James Alford-Golojuch. All rights reserved.
//

#include "nextStateMoves.hpp"
#include <vector>

using namespace std;

int nsPuzzleLength = 6;

vector<vector<vector<int>>> nsFlip(vector<vector<int>> upper, vector<vector<int>> lower){
    //for each column
    //take elements in each column in upper, put in temp
    //put lower into upper
    //put temp into lower
    int temp;
    for(int j=0; j<nsPuzzleLength; j++){
        for(int i=0; i<nsPuzzleLength; i++){
            temp = upper[j][i];
            upper[j][i] = lower[j][i];
            lower[j][i] = temp;
        }
    }
    //if (gravity == 0){ gravity = 1; }
    //else{ gravity = 0; }
    nsSettleBoard(upper, lower);
    vector<vector<vector<int>>> newState;
    newState.push_back(upper);
    newState.push_back(lower);
    return newState;
}

vector<vector<vector<int>>> nsClkRotate(vector<vector<int>> upper, vector<vector<int>> lower){
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
    nsSettleBoard(upper, lower);
    vector<vector<vector<int>>> newState;
    newState.push_back(upper);
    newState.push_back(lower);
    return newState;
}

vector<vector<vector<int>>> nsCclkRotate(vector<vector<int>> upper, vector<vector<int>> lower){
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
    nsSettleBoard(upper, lower);
    vector<vector<vector<int>>> newState;
    newState.push_back(upper);
    newState.push_back(lower);
    return newState;
}

void nsSettleBoard(vector<vector<int>>& upper, vector<vector<int>>& lower){
    //if it is not flipped and not settled, then we want to flip the non-negative portion of array and give it to other board
    
    //x is 0 to 12
    //y = x-6
    for(int i=0; i<6; i++){
        for(int k=0; k<6; k++){
            for(int j=0; j<11; j++){
                int currVal = nsGetValueAtPosition(i,j,upper,lower);
                int nextVal = nsGetValueAtPosition(i,j+1,upper,lower);
                if(currVal == -1){
                    continue;
                }
                if(currVal == 0){
                    if(nextVal == 0 || nextVal == -1){
                        continue;
                    }
                    nsSetValueAtPosition(i,j,nextVal,upper,lower);
                    nsSetValueAtPosition(i,j+1,currVal,upper,lower);
                    
                }
            }
        }
    }
}

void nsSetValueAtPosition(int i, int j, int input, vector<vector<int>>& upper, vector<vector<int>>& lower){
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

int nsGetValueAtPosition(int i, int j, vector<vector<int>> upper, vector<vector<int>> lower){
    
    int row = j-6;
    if(row < 0){
        row = -1-row;
        return lower[i][row];
    }
    else{
        return upper[i][row];
    }
}