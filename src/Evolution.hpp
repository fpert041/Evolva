//
//  Evolution.hpp
//  evolva~
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#ifndef Evolution_hpp
#define Evolution_hpp

#include <stdio.h>

#include <stdlib.h>
#include <iostream>

#include <vector>
#include <sstream>
#include <math.h>

#include "Individual.hpp"
#include "Population.hpp"

class Evolution {
    /* GA parameters */
private:
    static const double uniformRate = 0.5;
    static const double mutationRate = 0.0015;
    static const int tournamentSize = 10;
    static const bool elitism = false;
    
    /* Public methods */
public:
    
    // Evolve a population
    static Population evolvePopulation(Population);
    
    // Crossover individuals
    static Individual crossover(Individual, Individual);
    
    // Mutate an individual
    static void mutate(Individual);
    
    // Select individuals for crossover
    static Individual tournamentSelection(Population);
};


#endif /* Evolution_hpp */
