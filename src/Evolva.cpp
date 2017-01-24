//
//  Evolvaa.cpp
//  evolva
//
//  Created by Francesco Perticarari on 22/01/2017.
//
//

#include "Evolva.hpp"



// inherit from the MSP base class and a thread-building friendly template class:


Evolva::Evolva(t_symbol * sym, long ac, t_atom * av) {
    setupIO(2, 2);
#ifndef RANDOM_SEED
#define RANDOM_SEED
    static const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    
    solutionString = "000000000000100000000000101010110101100000000000";
    originalPop ="000000000000100000000000000000000000101010010011";
    
    newSol="000000000000000000000000000000000000101010010011";
    
    //Initialize goal solution and population
    Goals::setSolution(solutionString);
    myPopulation.reset(new Population(100, originalPop));
    notesToPlay = std::vector<int>();
    
    // Need just one note per 'bang' fow now
    notesPerUpdate = 1;
    
    post("object created");
}

Evolva::~Evolva() {
    post("object freed");
}


/*****************************************THREADING TRIAL start snippet*********************************************/

t_systhread thread; // thread ID
t_systhread_mutex	t_mutex; // mutual exclusion lock for threadsafety

// threadable function
void Evolva::thread_function(int notesPerUpdate, int bangInterval, std::vector<int>& notesToPlay)
{
    //    clock_t start = clock();
    //    long startTime = floor(((float)start)/CLOCKS_PER_SEC*1000);
    
    // X X X X DEBUGGING HERE X X X X X
    
    int notesPlayed = 0;
    //<<<<<<<<<<<<<<<< { *BUG IN MUTEX! }
    /////systhread_mutex_lock(t_mutex); //make sure that other threads cannot change critical variables whilst outputting
    if(!notesToPlay.empty()){ // make sure "notes to play" IS_NOT an empty list of notes
        while(notesPlayed < notesPerUpdate){
            outlet_int(m_outlets[0], notesToPlay[notesPlayed] + 24); // output notes as midi {TESTED :)}
            //        long ac = 200;
            //        t_atom * av;
            //        outlet_anything(m_outlets[0], gensym("test"), ac, av);
            notesPlayed++;
            if(notesPlayed < notesPerUpdate)
            {
                try
                {
                    systhread_sleep((long) (bangInterval / notesPerUpdate));
                }
                
                catch (ExceptionInformation ei)
                {
                    // TODO: appropriate catch block
                    unsigned long ul = ei.theKind;
                    post( " error code: %l ", ul );
                }
            }
        }
    }
    ////systhread_mutex_unlock(t_mutex);  //<<<<<<<<<<<<<<<< { *BUG IN MUTEX! }
    systhread_exit(0);
}



/* Create worker thread: this function gets called whenever a thread is created */
const void Evolva::run(){
    thread_function(notesPerUpdate, bangInterval, this->notesToPlay);
    
    post("flag: circular threading works %i",notesPerUpdate);
}



/*****************************************THREADING TRIAL end snippet*********************************************/

//---------------------------------------------------------------------------------------------
// INTERFACE


// (BANG) : called when a bang is received into an inlet
void Evolva::bang(long inlet) {
    
    //int i = 1 + (rand() % 6); /* random int between 1 and 6 */
    //double d = rand()/RAND_MAX; /* random double-float between 0 and 1 */
    
    // post(Goals::getSolution().c_str()); //DEBUGGING //
    // post(myPopulation->getFittest()->toString().c_str()); //DEBUGGING //
    // post(std::to_string( Goals::getMaxFitness() ).c_str()); //DEBUGGING //
    
    // Evolution the population by one step until we reach an optimum solution
    if(myPopulation->getFittest()->getFitness() < Goals::getMaxFitness())
    {
        myPopulation = Evolution::evolvePopulation(myPopulation);  // WATCH OUT * * * < Check there is appropriate variation/selevtion for evolutionary purposes - FITTEST's FITNESS DOES NOT SEEM TO GROW THAT WELL!
        generationCount++;
        std::string str = "Generation: " + std::to_string(generationCount) 
                        +  " Fittest: " + std::to_string(myPopulation->getFittest()->getFitness());
        post(str.c_str());
    }
    else
    {
        // Depending on your input, the result will be constant once the solution has been reached
        post("Solution found!");
        //			post( ("Generation: " + std::to_string(generationCount)).c_str() );
        //          post(("Genes: " + myPopulation->getFittest()->toString()).c_str());
    }
    
    // Set list of notes to play
    notesToPlay = chooseNotes(myPopulation->getFittest()->toString(), notesPerUpdate);
    
    // Play notes in a NEW THREAD:
    mcppthread_create(this, &thread); // pass object where to find appropriate run() function AND a thread ID address
            // -> systhread_create( (method) threaded_func, this, 0, 0, 0, &thread );
    
    
    post("bang in inlet %i!", inlet);
}

// (FLOAT) : called when a float is received into an inlet
void Evolva::inFloat(long inlet, double v) {
    post("inlet %ld float %f", inlet, v);
    
    bangInterval = v;
    
    //outlet_float(m_outlets[0], v);
}

// (INTEGER) : called when an int is received into an inlet
void Evolva::inInt(long inlet, long v) {
    post("inlet %ld int %ld", inlet, v);
    
    bangInterval = v;
    
    //outlet_int(m_outlets[0], v);
}


//---------------------------------------------------------------------------------------------

// Other interface methods: //

// Change the solution of the GA
void Evolva::setSolution(std::string newSolution)
{
    // Implement logic to change solution here
    
    if(newSolution.compare("red")) {
        newSol = "110010111001100000000000100000000000000000000000";
    } else if(newSolution.compare("blue")) {
        newSol = "000000000000101101011010100000000000000000000000";
    } else if(newSolution.compare("green")) {
        newSol = "000000000000000000000000101010110101100000000000";
    } else if(newSolution.compare("yellow")) {
        newSol = "000000000000000000000000000000000000101011010101";
    } else {
        post("Input string for 'newSolution(std::string)' is not recognized \nUse: 'red', 'blue', 'yellow' and 'green' instead");
    }
    
    // Set new fitness string
    Goals::setSolution(newSol);
    // Create a new population with previous string
    myPopulation.reset(new Population(100, solutionString));
    // Replace old solution string with new one
    solutionString = newSol;
    // Reset generation count
    generationCount = 0;
}

// Overload the 'setSolution' method to respond differently to different data types
void Evolva::setSolution(long a)
{
    std::string newSolution = std::to_string(a);
    generationCount = 0;
}


//---------------------------------------------------------------------------------------------
// HELPER METHODS

// Select and return a list of notes: PHENOTYPE
std::vector<int> Evolva::chooseNotes(std::string fittest, int howManyNotes)
    // params: (fittest individual, n# of notes to play)
{

    std::vector<int> possibleNotes = std::vector<int>(); // vector of possible notes to play
    for(int i=0; i<fittest.length(); ++i){
        if(fittest[i] == '1'){
            possibleNotes.push_back(i); // go through the genotype of fittest individual (current solution)...
                                        // ...and every time there is a "1", add its index to the possible notes vector
        }
    }

    std::vector<int> notesToPlay = std::vector<int>(); // vector for the notes that the object will output

    // Loop though the n# of notes specified as a param...
    for(int i=0; i < howManyNotes; i++){
        if(possibleNotes.size()>0){ // ...and if there are possible notes in memory...
            notesToPlay.push_back(possibleNotes[rand()%possibleNotes.size()]); //...push back a random int between 0 and the length of the possible notes vector
        } else { // otherwise...
            notesToPlay.push_back(1); //...push back 1
        }
    }

    return notesToPlay;
}

//---------------------------------------------------------------------------------------------


/* -- for MSP objects only (REMEMBER TO ADD IT IF THIS BECOMES ONE IN FUTURE!)---
 // default signal processing method is called 'perform'
 void perform(double **ins, long numins, double **outs, long numouts, long sampleframes) {
 // example code to invert inputs
 for (long channel = 0; channel < numouts; channel++) {
 double * in = ins[channel];
 double * out = outs[channel];
 for (long i=0; i < sampleframes; i++) {
 out[i] = -in[i];
 }
 }
 }
 */

