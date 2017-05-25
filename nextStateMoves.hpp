//
//  nextStateMoves.hpp
//  AIProgram2
//
//  Created by Jason  Sands on 9/16/16.
//  Copyright © 2016 James Alford-Golojuch. All rights reserved.
//

#ifndef nextStateMoves_hpp
#define nextStateMoves_hpp

#include <stdio.h>
#include <vector>

using namespace std;

vector<vector<vector<int>>> nsFlip(vector<vector<int>> upper, vector<vector<int>> lower);
vector<vector<vector<int>>> nsClkRotate(vector<vector<int>> upper, vector<vector<int>> lower);
vector<vector<vector<int>>> nsCclkRotate(vector<vector<int>> upper, vector<vector<int>> lower);
void nsSettleBoard(vector<vector<int>>& upper, vector<vector<int>>& lower);
void nsSetValueAtPosition(int i, int j, int input, vector<vector<int>>& upper, vector<vector<int>>& lower);
int nsGetValueAtPosition(int i, int j, vector<vector<int>> upper, vector<vector<int>> lower);


#endif /* nextStateMoves_hpp */
