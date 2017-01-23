//
//  Population.cpp
//  evolva
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#include "Population.hpp"


//--------------------------------------------------------------------------------------------
// CONSTRUCTORS

Population::Population(std::vector<std::string> originals){
    usePredefined = false;
    //individuals = std::vector<Individual>(originals.size());
    
    for(int i=0; i<originals.size(); ++i){
        std::shared_ptr<Individual> newbie(new Individual(originals[i])); // initialise individual on the heap (using a smart pointer)
        individuals.push_back(newbie); // push back individual's smart_pointer onto our vector of "individial"s on the stack
    }
}

Population::Population(int popSize, bool init){
    if(popSize < 2) popSize = 2; // minim population size
    
    usePredefined = false;
    if(init){
        if(usePredefined){
            std::shared_ptr<Individual> alpha(new Individual("101010101010"));
            std::shared_ptr<Individual> beta(new Individual("010010101101"));
            if(popSize%2 != 0) individuals.push_back(beta);
            
            for(int i=0; i<floor(popSize/2); ++i){
                individuals.push_back(alpha); // push back individual's smart_pointer onto our vector of "individial"s on the stack
                individuals.push_back(beta);
            }
        } else {
            // Loop and create individuals
            for (int i = 0; i < size(); i++)
            {
                std::shared_ptr<Individual> newbie(new Individual()); // initialise individual on the heap (using a smart pointer)
                individuals.push_back(newbie); // push back individual's smart_pointer onto our vector of "individial"s on the stack
            }
        }
    }
}

Population::Population(int popSize, std::string original){
    // post("flag!!!"); //DEBUGGING LINE <- use this to find out which constructor you are using 
    
    usePredefined = false;
    //individuals = std::vector<Individual>(popSize);
    for (int i = 0; i < size(); i++)
    {
        std::shared_ptr<Individual> newbie(new Individual(original)); // initialise individual on the heap (using a smart pointer)
        individuals.push_back(newbie);
    }
}

//--------------------------------------------------------------------------------------------
// DESTRUCTOR

Population::~Population() {}


//--------------------------------------------------------------------------------------------
// INTERFACE

// Get population size
int Population::size()
{
    return individuals.size();
}

// Save given individual by replacing the content of the smart pointer at the index provided
void Population::saveIndividual(int index, std::shared_ptr<Individual> alice)
{
    individuals[index].swap(alice);
}

// Get a smart pointer to the individual at a given index
std::shared_ptr<Individual> Population::getIndividual(int index){
    return individuals[index];
}

// Get a smart pointer to the fittest individual
std::shared_ptr<Individual> Population::getFittest(){
    // {ARIANA's THREAD 1} {Note: it seems that individuals[] is not pointing to an Individual
    post(std::to_string(size()).c_str()); // {THIS is returning 0}
    
    std::shared_ptr<Individual> fittest(new Individual()); //this works!
    //std::shared_ptr<Individual> fittest = individuals[0]; // the first being is initially the fittest...
    
//    for(int i=0; i<individuals.size(); ++i){
//        if(fittest->getFitness() <= individuals[i]->getFitness()){
//            fittest.swap(individuals[i]); // ...but, as we loop through our vector of beings, we compare their fitness and successively select a new fittest if we find one
//        }
//    }
    return fittest;
}




