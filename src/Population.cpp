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

Population::Population(std::vector<std::string> originals) : usePredefined(false), sorted(false){
    
    for(int i=0; i<originals.size(); ++i){
        std::shared_ptr<Individual> newbie(new Individual(originals[i])); // initialise individual on the heap (using a smart pointer)
        individuals.push_back(newbie); // push back individual's smart_pointer onto our vector of "individial"s on the stack
    }
}

Population::Population(int popSize, bool init) : individuals(popSize), usePredefined(false), sorted(false){
    if(popSize < 2) throw 11; // minim population size = 2
    
    // post("flag!!!"); //DEBUGGING LINE <- use this to find out which constructor you are using
    
    if(init){ // spawn an initial population....
        if(usePredefined){
            std::shared_ptr<Individual> alpha(new Individual("101010101010"));
            std::shared_ptr<Individual> beta(new Individual("010010101101"));
            
            // populate this object's array of beings with the above 2 individuals
            int i = 0;
            for(; i < size()/2; ++i){
                individuals[i] = alpha;
            } for (; i < size(); ++i){
                individuals[i] = beta;
            }
            
        } else {
            // Loop and create individuals
            for (int i = 0; i < size(); i++)
            {
                std::shared_ptr<Individual> newbie(new Individual()); // initialise individual on the heap (using a smart pointer)
                individuals[i] = newbie; // push back individual's smart_pointer onto our vector of "individial"s on the stack
            }
        }
    } /* end_if (init) */ //....otherwise, just create the object with an empty vector of size "popSize"
}

Population::Population(int popSize, std::string original) : individuals(popSize), usePredefined(false), sorted(false){
    
    // post("flag!!!"); //DEBUGGING LINE <- use this to find out which constructor you are using

        // individuals = std::vector<std::shared_ptr<Individual> >(popSize); // REPLACED BY ":" ABOVE FOR BEST/FASTER INITIALISAZION
    for (int i = 0; i < size(); i++)
    {
        std::shared_ptr<Individual> newbie(new Individual(original)); // initialise individual on the heap (using a smart pointer)
        individuals[i] = newbie;
    }
    
    // post(original.c_str());
    // post(getFittest()->toString().c_str());
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
    sorted = false;
    
    individuals[index].swap(alice);
}

// Get a smart pointer to the individual at a given index
std::shared_ptr<Individual> Population::getIndividual(int index){
    return individuals[index];
}

// Get a smart pointer to the fittest individual
std::shared_ptr<Individual> Population::getFittest(){
    
    // - - - - DEBUGGED HERE - - - -
    //post(std::to_string(size()).c_str()); // {THIS is returning 100 correctly}
    //post(individuals[0]->toString().c_str()); // {THIS is returning a binary string of 48 genes }
    
    std::shared_ptr<Individual> fittest = individuals[0]; // the first being is initially the fittest...
    
    for(int i=0; i<individuals.size(); ++i){
        if(fittest->getFitness() < individuals[i]->getFitness()){
            fittest = individuals[i]; // ...but, as we loop through our vector of beings, we compare their fitness and successively select a new fittest if we find one
        }
    }
    
    return fittest;
    //post(fittest->toString().c_str()); // { THIS is returning a binary string of 48 genes }
}

// Get a smart pointer to the fittest individual
std::vector<std::shared_ptr<Individual> > Population::getFittests(int n){
    
    if(!sorted)
        mergeSort();
    
    std::vector<std::shared_ptr<Individual> > v1(individuals.begin(), individuals.begin() + n);
    return v1;
    //...code
}

void Population::mergeSort(){
    std::vector<std::shared_ptr<Individual> > temp = this->individuals;
    
    TopDownSplitMerge(this->individuals, 0, temp.size(),  temp);
    sorted = true;
}





//--------------------------------------------------------------------------------------------

// HELPERS

void Population::TopDownSplitMerge(std::vector<std::shared_ptr<Individual> >& input, int left, int right, std::vector<std::shared_ptr<Individual> >& scratch){
    
    
    /* base case: one element */
    if(right == left + 1)
    {
        return;
    }
    
    int i = 0;
    int length = right - left;
    int midpoint_distance = length/2;
    /* l and r are to the positions in the left and right subarrays */
    int l = left, r = left + midpoint_distance;
    
    /* sort each subarray */
    TopDownSplitMerge(input, left, left + midpoint_distance, scratch);
    TopDownSplitMerge(input, left + midpoint_distance, right, scratch);
    
    /* merge the arrays together using scratch for temporary storage */
    for(i = 0; i < length; i++)
    {
        /* Check to see if any elements remain in the left array; if so,
         * we check if there are any elements left in the right array; if
         * so, we compare them.  Otherwise, we know that the merge must
         * use take the element from the left array */
        if(l < left + midpoint_distance &&
           (r == right || maxi(input[l]->getFitness(), input[r]->getFitness()) == input[l]->getFitness()))
        {
            scratch[i] = input[l];
            l++;
        }
        else
        {
            scratch[i] = input[r];
            r++;
        }
    }
    /* Copy the sorted subarray back to the input */
    for(i = left; i < right; i++)
    {
        input[i] = scratch[i - left];
    }
    
}

/* Helper function for finding the max of two numbers */
int Population::maxi(int x, int y)
{
    if(x > y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

