//
//  Individual.hpp
//  evolva
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#ifndef Individual_hpp
#define Individual_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <Math.h>

#include "FitnessFuncs.hpp"

class Individual{
private:
    
    int defaultGeneLength = 84;
    std::vector<int> genes; // Vector to store genes
    
    // Score
    int fitness = 0;
    
protected:
    
    int getFitness(Individual*);
    
public:
    Individual(); // default constructor
    ~Individual(); // destructor
    
    Individual(std::string); // constructor taking in a genotype

    void newBeing(); // Create a random individual
    
    void newBeing(std::string); // constrained to string-genotype
    
    //---------------------------------------------------------
    
    // Use this if you want to create individuals with different gene lengths
    void setDefaultGeneLength(int length);
    
    int getGene(int index);
    
    void setGene(int index, int value);
    
    int size(); // return genes.length; (genetic "size" of this individual)
    
    int getFitness(); // return fitness;
    
    std::string toString(); // return genotype as a string

};

#endif /* Individual_hpp */
