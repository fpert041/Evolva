//
//  Individual.cpp
//  evolva
//
//  Created by Francesco Perticarari on 18/01/2017.
//
//

#include "Individual.hpp"


//--------------------------------------------------------------------------------------------
// CONSTRUCTORS

Individual::Individual(){
    
#ifndef RANDOM_SEED
#define RANDOM_SEED
    static const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
#endif
    
    genes = std::vector<int>(defaultGeneLength); // default genotype length
    
    srand(seed); /* seed random number generator */
}

Individual::Individual(std::string genotype){
    
#ifndef RANDOM_SEED
#define RANDOM_SEED
    static const int seed = time(NULL);
#endif
    
    genes = std::vector<int>(genotype.length()); // Genes-vector length initialised according to the input genotype
    
    // Loop through each character of the input string and save it in our vector as an int
    for(int i=0; i<genotype.length(); ++i){
        if(strcmp(&genotype[i], "1")){
           genes[i]=1;
        } else {
           genes[i]=0;
        }
    }
}

//--------------------------------------------------------------------------------------------

// GENERATE NEW INDIVIDUAL WITH A RANDOM GENOTYPE

void Individual::newBeing(){
    for(int i=0; i<genes.size(); ++i){
        genes[i] = floor(rand()/RAND_MAX +0.5);
    }
}


void Individual::newBeing(std::string genotype){

    genes = std::vector<int>(genotype.length()); // Genes-vector length re-initialised according to the input genotype
    
    for(int i=0; i<genes.size(); ++i){
        if(strcmp(&genotype[i],"1")){
            genes[i] = floor(rand()/RAND_MAX +0.85); // -> Allow random mutation but keep it unlikely
        }
    }
}

//--------------------------------------------------------------------------------------------

// INTERFACE METHODS: Settings + Gettings

void Individual::setDefaultGeneLength(int length){
    defaultGeneLength = length;
}

int Individual::getGene(int index){
    return genes[index];
}

void Individual::setGene(int index, int value){
    genes[index]=value;
}

int Individual::size(){
    return genes.size();
}

std::string Individual::toString(){
    std::string genotype = "";
    
    for(int i=0; i<size(); ++i){
        genotype += std::to_string(getGene(i));
    }
    
    return genotype;
}

//--------------------------------------------------------------------------------------------

// GET FITNESS

int Individual::getFitness(){
    
    if(fitness != 0){
        fitness = getFitness(this);
    }
    
    return fitness;
}

int Individual::getFitness(Individual* alice){
    
    //int ex = Fitness::solution[3]; // debug test
    
    int fitness = 0;
    
     //Loop through our individuals genes and compare them to our candidates
            for(int i=0; i<alice->size() && i<Fitness::solution.size(); ++i){
                //do...
            }
    
    return fitness;
}







