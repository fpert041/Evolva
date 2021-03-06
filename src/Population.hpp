//
//  Population.hpp
//  evolva
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#ifndef Population_hpp
#define Population_hpp

#include "maxcpp.h"

#include <stdio.h>
#include <vector>
#include <string>

#include "Individual.hpp"

class Population{
    
private:
    std::vector<std::shared_ptr<Individual> > individuals;
    bool usePredefined;
    
    static void TopDownSplitMerge(std::vector<std::shared_ptr<Individual> >& A, int, int, std::vector<std::shared_ptr<Individual> >& B); // helper method for merge sort
    
    static int maxi(int x, int y); // return maximum between two integers
 
public:
    /*
     * Constructors:
     *  Create a population
     */
    Population(std::vector<std::string>); // from an original population input as a vector of string
    
    Population(int , bool ); // from size and "usePredefined" value
    
    Population(int , std::string); // from constraining string value
    
    ~Population(); // DESTRUCTOR
    
    /* Getters */
    std::shared_ptr<Individual> getIndividual(int); // return individuals[index];
    
    std::shared_ptr<Individual> getFittest(); // return fittest
    
    // Get a smart pointer to the <INT> fittest individuals
    std::vector< std::shared_ptr<Individual> >getFittests(int);
    
    /* Public methods */
    // Get population size
    int size(); // return individuals.length;
    
    // Sort population
    void mergeSort();
    
    bool sorted;
    
    // Save individual
    void saveIndividual(int, std::shared_ptr<Individual>); // params: (index, indiv) => individuals[index] = indiv;
    
};

#endif /* Population_hpp */
