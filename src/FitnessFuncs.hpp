//
//  FitnessFuncs.hpp
//  evolva
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#ifndef FitnessFuncs_hpp
#define FitnessFuncs_hpp

#include <stdio.h>
#include <string>
#include <vector>


/* GLOBAL FITNESS CALCULATION METHODS & SOLUTION DATA */

namespace Fitness {
    
    extern int solutionSize;
    extern std::vector<int>solution;
    
    // Set a candidate solution as an int array
    extern void setSolution(int[]);
    
    // To make it easier we can use this method to set our candidate solution
    // with string of 0s and 1s
    extern void setSolution(std::string);
    
    // Calculate inidividual's fitness by comparing it to our candidate solution
    ////extern int getFitness(Individual*);
    
    // Get optimum fitness
    extern int getMaxFitness();
    
};

#endif /* FitnessFuncs_hpp */
