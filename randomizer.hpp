//
//  randomizer.hpp
//  AIProgram2
//
//  Created by Jason  Sands on 9/16/16.
//  Copyright © 2016 James Alford-Golojuch. All rights reserved.
//

#ifndef randomizer_hpp
#define randomizer_hpp

#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include <vector>
#include <stdlib.h>

using namespace std;


vector<vector<vector<int>>> shuffleBoard(int k);
bool isFlat();
bool isSymmetric();
void givenLegalMove(char move);
void flip();
void clkRotate();
void cclkRotate();
void unknownLegalMove();
void settleBoard();
void setValueAtPosition(int i, int j, int input);
int getValueAtPosition(int i, int j);
int offsetLower();
int offsetUpper();
void assignColors();
void loadState(int i);

#endif /* randomizer_hpp */
