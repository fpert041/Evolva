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
    const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    fitness = 0;

    genes = std::vector<int>(defaultGeneLength); // default genotype length
    
    newBeing(); // random initialisation
}


Individual::Individual(bool lengthVariation){ // possible TODO: A BOOL LIKE THIS COULD BE INSERT IN ALL CONSTRUCTORS
    
#ifndef RANDOM_SEED
#define RANDOM_SEED
    const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    fitness = 0;
    
    int length = defaultGeneLength;
    
    if(lengthVariation){ // if genotype variations are allowed: randomise genotype length wothin reason
        length += rand()%2;
    }
    
    genes = std::vector<int>(length); // non necessarily fixed genotype length
    
    
    newBeing(); // random initialisation
}


Individual::Individual(std::string genotype){
    
#ifndef RANDOM_SEED
#define RANDOM_SEED
    const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    fitness = 0;
    
    genes = std::vector<int>(genotype.length()); // Genes-vector length initialised according to the input genotype
    
    // Loop through each character of the input string and save it in our vector as an int
    for(int i=0; i<genotype.length(); ++i){
        if(strncmp(&genotype[i], "1", 1) == 0){
           genes[i]=1;
        } else {
           genes[i]=0;
        }
    }
}


Individual::Individual(int genotypeSize){
    
#ifndef RANDOM_SEED
#define RANDOM_SEED
    const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    fitness = 0;
    
    genes = std::vector<int>(genotypeSize); // Genes-vector length initialised according to the input genotype
    
    newBeing(); // random initialisation

}

//--------------------------------------------------------------------------------------------

// DESTRUCTOR

Individual::~Individual(){}


//--------------------------------------------------------------------------------------------

// GENERATE NEW INDIVIDUAL WITH A RANDOM GENOTYPE

void Individual::newBeing(){
    for(int i=0; i<genes.size(); ++i){
        genes[i] = floor(rand()/(double)RAND_MAX +0.5);
    }
}


void Individual::newBeing(std::string genotype){

    genes = std::vector<int>(genotype.length()); // Genes-vector length re-initialised according to the input genotype
    
    for(int i=0; i<genes.size(); ++i){
        if(strncmp(&genotype[i],"1", 1) == 0){
            genes[i] = floor(rand()/(double)RAND_MAX +0.85); // -> Allow random mutation but keep it unlikely
        }
    }
}

//--------------------------------------------------------------------------------------------

// INTERFACE METHODS: Settings + Gettings

void Individual::setDefaultGeneLength(int length){
    defaultGeneLength = length;
}

int Individual::getGene(int index){
    if(index<size()){ // avoid out-of-bounds issues
        return genes[index];
    } else {
        return 0;
    }
}

void Individual::setGene(int index, int value){
    
    if(index<size()){ // avoid out-of-bounds issues
        genes[index]=value;
    } else {
        genes.push_back(value);
    }
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
    
    if(fitness == 0){
        fitness = getFitness(this);
    }
    
    return fitness;
}

int Individual::getFitness(Individual* alice){
    
    //int ex = Fitness::solution[3]; // debug test
    
    int fitness = 0;
    
     //Loop through our individuals genes and compare them to the ideal candidate's
            for(int i=0; i<alice->size() && i<Goals::solution.size(); ++i){
                if(alice->getGene(i) == Goals::solution[i]){
                    fitness++;
                }
            }
    
    return fitness;
}







