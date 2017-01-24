//
//  FitnessFuncs.cpp
//  evolva
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#include "FitnessFuncs.hpp"

#include <iostream>

namespace Goals{
    
    //class Individual; // forward declaration
    
    int solutionSize = 12;
    std::vector<int>solution(solutionSize);

    // Set a candidate solution as a std::int vector
    void setSolution(std::vector<int> newSolution){
        solution = newSolution;
        post("new solution set");
    }
    
    // Set a candidate solution using a std::string of 1s and 0s
    void setSolution(std::string newSolution) {
        
        solution = std::vector<int>(newSolution.length());
        
        for (int i = 0; i < solution.size(); ++i){
            solution[i] = strncmp(&newSolution[i],"0", 1) ? 0 : 1;
        }
        post("new solution set with a string");
    }
    
    // Get optimum fitness
    int getMaxFitness(){
        int maxFitness = solution.size();
        return maxFitness;
    }
    
    std::string getSolution(){
        std::string out = "";
        for(int i = 0; i<solution.size(); i++){
            out += std::to_string(solution[i]);
        }
        return out;
    }
};
