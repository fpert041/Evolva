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
const double Evolution::mutationChance = 0.10;
const int Evolution::fightersForSurvival = 50;
bool Evolution::elitism = true;

//--------------------------------------------------------------------------------------------

/* Interface */

// Evolve a population and return a pointer to the new population object
std::shared_ptr<Population> Evolution::evolvePopulation(std::shared_ptr<Population> pop){
    
    // WATCH OUT * * * < Check there is appropriate Evolutionary drive
    
    std::shared_ptr<Population> newPopulation(new Population(pop->size(), false));
   
    pop->bubbleSort();
    int leaders = 4;
    
    // Keep the fittest
    if(elitism){
        for(int i= 0; i<leaders; ++i){
            newPopulation->saveIndividual(0, pop->getIndividual(i));
        }
    }
    
    // Natural Selection + Reproduction with (Crossover + random mutation)
    int leaderSeat = (elitism == true ? leaders : 0);
    
    for (int i = leaderSeat; i < pop->size(); i++) // WATCH OUT * * * < Check there is appropriate variation/selevtion
    {
        
        std::shared_ptr<Individual> alpha = naturalSelection(pop);
        
        // int randomIndex = rand() % pop->size();
        // std::shared_ptr<Individual> beta = pop->getIndividual(randomIndex);
        std::shared_ptr<Individual> beta = naturalSelection(pop);
        
        std::shared_ptr<Individual> offspring = crossover(alpha, beta);
        newPopulation->saveIndividual(i, offspring);
        
        mutate(newPopulation->getIndividual(i));
    }
    
    post(std::to_string(newPopulation->getFittest()->getFitness()).c_str());
    post(std::to_string(newPopulation->getIndividual(0)->getFitness()).c_str());
    post(std::to_string(newPopulation->getIndividual(1)->getFitness()).c_str());
    post(std::to_string(newPopulation->getIndividual(10)->getFitness()).c_str());
    post(std::to_string(newPopulation->getIndividual(20)->getFitness()).c_str());
    post(std::to_string(newPopulation->getIndividual(50)->getFitness()).c_str());
    post(std::to_string(newPopulation->getIndividual(90)->getFitness()).c_str());
    
//    post(std::to_string(newPopulation->size()).c_str()); //DEBUGGING // OK
//    post(newPopulation->getFittest()->toString().c_str()); // DEBUGGING // OK
//    post(std::to_string( newPopulation->getFittest()->getFitness()).c_str()); // DEBUGGING // OK
    
    pop.reset(); // Free heap memory where the old population was
     
    return newPopulation; // Return pointer to the new Population object
    
}

//--------------------------------------------------------------------------------------------

// HELPER FUNCTIONS

// Crossover individuals
std::shared_ptr<Individual> Evolution::crossover(std::shared_ptr<Individual> alpha, std::shared_ptr<Individual> beta){
#ifndef RANDOM_SEED
#define RANDOM_SEED
    const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    
    std::shared_ptr<Individual> newbie( new Individual());
    
    // Loop through genes...
    for(int i=0; i<alpha->size(); ++i){
        // ...and apply crossover
        newbie->setGene(i, ( rand()/(double)RAND_MAX < crossoverBias ? alpha->getGene(i) : beta->getGene(i) ));
    }
    
    return newbie;
}

// Mutate individuals' genotypes
void Evolution::mutate(std::shared_ptr<Individual> gazorpazorp){
#ifndef RANDOM_SEED
#define RANDOM_SEED
    const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    
    // Loop through genes...
    for(int i=0; i<gazorpazorp->size(); ++i){
        // ...and (possibly) mutate some genes at random
        if(rand()/(double)RAND_MAX < mutationChance){
            int gene = rand()%2;
            gazorpazorp->setGene(i, gene);
        }
    }
}

// Select individuals for crossover
std::shared_ptr<Individual> Evolution::naturalSelection(std::shared_ptr<Population> pop){
    
    
    int i =0;
    
    // Create a pool of individuals to select from
    Population genePool(fightersForSurvival, false);
    
    // For each place in the gene pool get a random individual from the input population
    for(; i<fightersForSurvival/3; ++i){
        genePool.saveIndividual(i, pop->getIndividual(i));
        
    } for (; i<fightersForSurvival; ++i){
        int randomIndex = rand() % pop->size();
        genePool.saveIndividual(i, pop->getIndividual(randomIndex));
    }
    
    // Get the fittest
    std::shared_ptr<Individual> fittest = genePool.getFittest();
    
    //post(std::to_string(genePool.getFittest()->getFitness()).c_str());
    
    return fittest;
}
