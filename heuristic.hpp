//
//  heuristic.hpp
//  AIProgram2
//
//  Created by Jason  Sands on 9/16/16.
//  Copyright © 2016 James Alford-Golojuch. All rights reserved.
//

#ifndef heuristic_hpp
#define heuristic_hpp

#include <stdio.h>
#include <vector>

using namespace std;

bool hIsFlat(vector<vector<int>> upper);
bool hIsSolved(vector<vector<int>> lower);
int heuristicValue(vector<vector<int>> upper, vector<vector<int>> lower);
int hOffsetLower(vector<vector<int>> lower);
int hOffsetUpper(vector<vector<int>> upper);

#endif /* heuristic_hpp */
