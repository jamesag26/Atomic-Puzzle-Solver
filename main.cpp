//
//  main.cpp
//  AIProgram2
//
//  Created by Jason  Sands on 9/15/16.
//  Copyright © 2016 James Alford-Golojuch. All rights reserved.
//

#include "randomizer.hpp"
#include "heuristic.hpp"
#include "nextStateMoves.hpp"

#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <queue>
#include <stdlib.h>

using namespace std;
int puzzleLength1 = 6;

char ballColor1 [22] = { '~',
    'r','r','r','r','r','r',
    'g','g','g','g','g',
    'y','y','y','y',
    'b','b','b',
    'w','w',
    'n'};

class statesValues {
public:
    int g;
    int h;
    vector<vector<int>> upperState = {{0,0,0,0,0,0},{0,0,0,0,0,-1},{0,0,0,0,-1,-1},{0,0,0,-1,-1,-1},{0,0,-1,-1,-1,-1},{0,-1,-1,-1,-1,-1}};
    vector<vector<int>> lowerState = {{0,0,0,0,0,0},{0,0,0,0,0,-1},{0,0,0,0,-1,-1},{0,0,0,-1,-1,-1},{0,0,-1,-1,-1,-1},{0,-1,-1,-1,-1,-1}};
    bool wasFlip;
    
    statesValues(int tempG, int tempH, vector<vector<int>> upperTempState, vector<vector<int>> lowerTempState, bool lastMove){
        g = tempG;
        h = tempH;
        upperState = upperTempState;
        lowerState = lowerTempState;
        wasFlip = lastMove;
    }
    
    bool operator< (statesValues const& other) const
    {
        return (g + h) < (other.g + other.h);
    }
    
    bool operator> (statesValues const& other) const
    {
        return (g + h) > (other.g + other.h);
    }
};

void printBoard1(vector<vector<int>> newUpper, vector<vector<int>> newLower){
    for(int i = puzzleLength1-1; i >= 0; i--){
        for(int j = 0; j < puzzleLength1; j++){
            if(newUpper[j][i] != -1){
                //removing ballColor --e.g. upper[j][i] will give ball numbers only
                cout << ballColor1[newUpper[j][i]] << " ";
            }else{
                //". " clarifies -1, unfillable spaces.
                cout << "  ";
            }
        }
        cout << endl;
    }
    for(int i = 0; i < puzzleLength1; i++){
        for(int j = 0; j < puzzleLength1; j++){
            if(newLower[j][i] != -1){
                cout << ballColor1[newLower[j][i]] << " ";
            }else{
                cout << ". ";
            }
        }
        cout << endl;
    }
}

//Moves for finding next state


int main() {
    //Randomize state to be at least k states away from solution
    int k = 8;
    //Initial State
    vector<vector<int>> initLower({{1,2,3,4,5,6},{7,8,9,10,11,-1},{12,13,14,15,-1,-1},{16,17,18,-1,-1,-1},{19,20,-1,-1,-1,-1},{21,-1,-1,-1,-1,-1}});
    vector<vector<int>> initUpper({{0,0,0,0,0,0},{0,0,0,0,0,-1},{0,0,0,0,-1,-1},{0,0,0,-1,-1,-1},{0,0,-1,-1,-1,-1},{0,-1,-1,-1,-1,-1}});
    //Vector to hold state of both upper and lower tube for root node and children nodes
    vector<vector<vector<int>>> randomState;
    vector<vector<vector<int>>> tempState;
    randomState.push_back(initUpper);
    randomState.push_back(initLower);
    tempState.push_back(initUpper);
    tempState.push_back(initLower);
    //Only does one instance at a time
    for (int i = 0; i < 1; i++){
        //Keeps track of how far down the tree we are
        int gCount = 0;
        //Checks if state is solved
        bool solved = false;
        //Priority queue for states
        priority_queue<statesValues, vector<statesValues>, greater<statesValues>> states;
        
        //Shuffles board to random state to start
        randomState = shuffleBoard(k);
        printBoard1(randomState[0], randomState[1]);
        //Finds Heuristic value for start state and push
        int h1 = heuristicValue(randomState[0], randomState[1]);
        //states.push({gCount, h1, randomState[0], randomState[1], false});
        //cout << "The heuristic value for this state is " << h1 << endl;
        //Empty priority queue
        while (!states.empty()){
            states.pop();
        }
        int iteration = 4;
        while (!solved && iteration <= k){
            gCount = 0;
            int parentNodes = 0;
            states.push({gCount, h1, randomState[0], randomState[1], false});
            //Loops through branches generating states and checking for solved states
            while (!solved && !states.empty()){
                //cout << "Tree Branch Level " << gCount << endl;
                int tempH = 0;
                statesValues tempStateVals = states.top();
                states.pop();
                if (tempStateVals.h == 0){
                    solved = true;
                    cout << "The puzzle has been solved" << endl;
                }
                //Move down to next branch of tree
                gCount = tempStateVals.g + 1;
                //Generates states for the 3 nodes from parent node if the max depth hasnt been reached
                if (gCount < iteration) {
                    //Parent nodes increase when children are generated from it
                    parentNodes++;
                    //For flip node
                    if (!tempStateVals.wasFlip){
                        tempState = nsFlip(tempStateVals.upperState, tempStateVals.lowerState);
                        tempH = heuristicValue(tempState[0], tempState[1]);
                        states.push({gCount, tempH, tempState[0], tempState[1], true});
                        // Proved to me node was being pushed
                        //cout << "Tree Branch Level " << gCount << endl;
                    }
                    //For clockwise node
                    tempState = nsClkRotate(tempStateVals.upperState, tempStateVals.lowerState);
                    tempH = heuristicValue(tempState[0], tempState[1]);
                    states.push({gCount, tempH, tempState[0], tempState[1], false});
                    // Proved to me node was being pushed
                    //cout << "Tree Branch Level " << gCount << endl;
                
                    //For counterclockwise node
                    tempState = nsCclkRotate(tempStateVals.upperState, tempStateVals.lowerState);
                    tempH = heuristicValue(tempState[0], tempState[1]);
                    states.push({gCount, tempH, tempState[0], tempState[1], false});
                    // Proved to me node was being pushed
                    //cout << "Tree Branch Level " << gCount << endl;
                }
                cout << "The nodes expanded on was " << parentNodes << endl;
                if (states.empty()) { cout << "The states list has been emptied with no solution" << endl; }
            }
            iteration = iteration + 2;
            cout << "Iteration" << iteration << endl;
        }
                        
        //printBoard1(tempState.upperState, tempState.lowerState);
        //cout << "The heuristic value for this state is " << tempState.h << endl;
        cout << "=========================== \n";
        randomState[0] = initUpper;
        randomState[1] = initLower;
    }
    return 0;
}
