//
//  Population.hpp
//  evolva
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#ifndef Population_hpp
#define Population_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include "Individual.hpp"

class Population{
    
private:
    std::vector<Individual> individuals = std::vector<Individual>();
    bool usePredefined = false;
    
    /*
     * Constructors
     */
    // Create a population
    Population(std::string); // from original population string array
    
    Population(int , bool ); // from size and "usePredefined" value
    
    Population(int , std::string); // from constraining string value
    
    /* Getters */
    Individual getIndividual(int index); // return individuals[index];
    
    Individual getFittest(); // return fittest
    
    /* Public methods */
    // Get population size
    int size(); // return individuals.length;
    
    // Save individual
    void saveIndividual(int, Individual); // params: (index, indiv) => individuals[index] = indiv;

    
};

#endif /* Population_hpp */
