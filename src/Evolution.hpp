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

struct Evolution {
    /* GA parameters */
private:
    static const double crossoverBias;
    static const double mutationChance;
    static const int fightersForSurvival;
    
    /* GA internal methods */
    // Crossover individuals
    static std::shared_ptr<Individual> crossover(std::shared_ptr<Individual>, std::shared_ptr<Individual>);
    
    // Mutate an individual
    static void mutate(std::shared_ptr<Individual>);
    
    // Select individuals for crossover
    static std::shared_ptr<Individual> naturalSelection(std::shared_ptr<Population>);
    
    
    /* Interface */
public:
    // If TRUE: allow for the fittest to lead the species and remain unaltered
    static bool elitism;
    
    // Evolve a population
    static std::shared_ptr<Population> evolvePopulation(std::shared_ptr<Population>);
};


#endif /* Evolution_hpp */
