//
//  Evolution.cpp
//  evolva~
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#include "Evolution.hpp"


/* GA parameters */
const double Evolution::crossoverBias = 0.5;
const double Evolution::mutationChance = 0.0015;
const int Evolution::fightersForSurvival = 10;
bool Evolution::elitism = false;


//--------------------------------------------------------------------------------------------

/* Interface */

// Evolve a population and return a pointer to the new population object
std::shared_ptr<Population> Evolution::evolvePopulation(std::shared_ptr<Population> pop){
    
    std::shared_ptr<Population> newPopulation(new Population(pop->size(), false));
    
    // Keep the fittest
    if(elitism){
        newPopulation->saveIndividual(0, pop->getFittest());
    }
    
    // Natural Selection + Reproduction with (Crossover + random mutation)
    int leaderSeat = (elitism == true ? 1 : 0);
    for (int i = leaderSeat; i < pop->size(); i++)
    {
        std::shared_ptr<Individual> alpha = naturalSelection(pop);
        std::shared_ptr<Individual> beta = naturalSelection(pop);
        
        std::shared_ptr<Individual> offspring = crossover(alpha, beta);
        newPopulation->saveIndividual(i, offspring);
        mutate(newPopulation->getIndividual(i));
    }
    
    
    pop.reset(); // Free heap memory where the old population was
    
    return newPopulation; // Return pointer to the new Population object
}

//--------------------------------------------------------------------------------------------

// HELPER FUNCTIONS

// Crossover individuals
std::shared_ptr<Individual> Evolution::crossover(std::shared_ptr<Individual> alpha, std::shared_ptr<Individual> beta){
    std::shared_ptr<Individual> newbie( new Individual());
    
    // Loop through genes...
    for(int i=0; i<alpha->size(); ++i){
        // ...and apply crossover
        newbie->setGene(i, ( rand()/RAND_MAX < crossoverBias ? alpha->getGene(i) : beta->getGene(i) ));
    }
    
    return newbie;
}

// Mutate individuals' genotypes
void Evolution::mutate(std::shared_ptr<Individual> gazorpazorp){
    // Loop through genes...
    for(int i=0; i<gazorpazorp->size(); ++i){
        // ...and (possibly) mutate some genes at random
        if(rand()/RAND_MAX < mutationChance){
            int gene = rand()%2;
            gazorpazorp->setGene(i, gene);
        }
    }
}

// Select individuals for crossover
std::shared_ptr<Individual> Evolution::naturalSelection(std::shared_ptr<Population> pop){
    
    // Create a pool of individuals to select from
    Population genePool(fightersForSurvival, false);
    // For each place in the gene pool get a random individual from the input population
    for(int i =0; i<fightersForSurvival; ++i){
        int randomIndex = rand() % pop->size();
        genePool.saveIndividual(i, pop->getIndividual(randomIndex));
    }
    // Get the fittest
    std::shared_ptr<Individual> fittest = genePool.getFittest();
    
    return fittest;
}
