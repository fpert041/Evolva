//
//  Evolvaa.cpp
//  evolva
//
//  Created by Francesco Perticarari on 22/01/2017.
//
//

#include "Evolva.hpp"



// inherit from the MSP base class and a thread-building friendly template class:


Evolva::Evolva(t_symbol * sym, long ac, t_atom * av) :
        solutionString("000000000000100000000000101010110101100000000000"),
        originalPop("000000000000100000000000000000000000101010010011"),
        newSol("000000000000000000000000000000000000101010010011"),
        notesPerUpdate(2), // 2 note per 'bang' fow now
        notesInterval (500) // Set how many milliseconds between notes
{
    setupIO(4, 6);
#ifndef RANDOM_SEED
#define RANDOM_SEED
    static const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
    srand(seed); /* seed random number generator */
#endif
    
    //Initialize goal solution and population
    Goals::setSolution(solutionString);
    myPopulation.reset(new Population(100, originalPop));
    
    for(int i = 0; i<50; ++i){
        myPopulation->getFittest()->newBeing();
    }
    
    // post(myPopulation->getFittest()->toString().c_str());
    
    notesToPlay = std::vector<int>();
    post("object created");
}

Evolva::~Evolva() {
    post("object freed");
}


/***************************************** THREADING SECTION start *********************************************/

t_systhread thread; // thread ID
t_systhread_mutex	t_mutex; // mutual exclusion lock for threadsafety

// threadable function
void Evolva::thread_function(int notesPerUpdate, int notesInterval, std::vector<int>& notesToPlay)
{
    //    clock_t start = clock();
    //    long startTime = floor(((float)start)/CLOCKS_PER_SEC*1000);
    
    // X X X X DEBUGGING HERE X X X X X
    
    int notesPlayed = 0;
    if(!notesToPlay.empty()){ // make sure "notes to play" IS_NOT an empty list of notes
        while(notesPlayed < notesPerUpdate){
            
            ////systhread_mutex_lock(t_mutex); //make sure that other threads cannot change critical variables whilst outputting //<<<<<<<<<<<<<<<< { *BUG IN MUTEX! }
            
            outlet_int(m_outlets[0], notesToPlay[notesPlayed] + 24); // output notes as midi {TESTED :)}
            notesPlayed++;
            
            ////systhread_mutex_unlock(t_mutex);  //<<<<<<<<<<<<<<<< { *BUG IN MUTEX! }
            
            if(notesPlayed < notesPerUpdate)
            {
                try
                {
                    systhread_sleep((long) (notesInterval / notesPerUpdate));
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
    systhread_exit(0);
}



/* Create worker thread: this function gets called whenever a thread is created */
const void Evolva::run(){
    thread_function(notesPerUpdate, notesInterval, this->notesToPlay);
    
    // post("flag: circular threading works %i",notesPerUpdate);
}



/***************************************** THREADING SECTION end *********************************************/

//---------------------------------------------------------------------------------------------
// INTERFACE


// (BANG) : called when a bang is received into an inlet
void Evolva::bang(long inlet) {
    
    
    
    switch (inlet){
        case (0):
            nextGeneration(); // evolve the population
            express(); // choose and load notes to be output
            output(); // launch a new thread and out put notes
            break;
            
        case(1):
            nextGeneration();
            break;
            
        case(2):
            express();
            break;
            
        case(3):
            express();
            output();
            break;
    }
    
    //post("bang in inlet %i!", inlet);
}

// (FLOAT) : called when a float is received into an inlet
void Evolva::inFloat(long inlet, double v) {
    
    switch (inlet) {
        case 0:
            if(v>0 && v<6) notesPerUpdate = v; //change notes output after each bang --there is a safety ceiling of 6
            break;
            
        case 1:
            if(v>50) notesInterval = v;
            break;
    }
    
    // post("inlet %ld float %f", inlet, v);
    // outlet_float(m_outlets[0], v);
}

// (INTEGER) : called when an int is received into an inlet
void Evolva::inInt(long inlet, long v) {
    
    switch (inlet) {
        case 0:
            if(v>0) notesPerUpdate = v;
            break;
            
        case 1:
            if(v>50) notesInterval = v;
            break;
    }
    
    
    // post("inlet %ld int %ld", inlet, v);
    // outlet_int(m_outlets[0], v);
}


//---------------------------------------------------------------------------------------------

// Other interface methods: //

// Change the solution of the GA
void Evolva::setSolution(long inlet, t_symbol * s, long ac, t_atom * av)
{
    if(inlet>0) return;
    
    // post( std::to_string(atom_getfloat(av)).c_str() );
    
    // Implement logic to change solution here:
    char* newSolution =  atom_string(av);//atom_getsym(av)->s_name;
    if(*newSolution == '\0') {
        post("Input string for 'newSolution(std::string)' is not recognized \nUse: 'red', 'blue', 'yellow' and 'green' instead");
        return;
    }
    
    if(strcmp(newSolution,"red") == 0) {
        newSol = "110010111001100000000000100000000000000000000000";
    } else {
        if(strcmp(newSolution,"blue") == 0) {
            newSol = "000000000000101101011010100000000000000000000000";
        } else {
            if(strcmp(newSolution,"green") == 0) {
                newSol = "000000000000000000000000101010110101100000000000";
            } else {
                if(strcmp(newSolution,"yellow") == 0) {
                    newSol = "000000000000000000000000000000000000101011010101";
                } else {
                    int counter = 0;
                    while(*newSolution != '\0') {
                        if(*newSolution != '0' || *newSolution !='1' ) { // << this was supposed to get the string in from Max but it turns out Max does not allow you to have messages containing binary strings as a number -- think of something else :p
                            post("Input string for 'newSolution(std::string)' is not recognized \nUse: 'red', 'blue', 'yellow' and 'green' instead");
                            return;
                        }
                        newSolution++;
                        counter++;
                    }
                    newSolution-=counter;
                    newSol = *newSolution;
                }
            }
        }
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
// TIP: TRY TO GO DOWN ANOTHER ROUTE -THIS MAY CREATE ISSUES WITH MAX's REGISTRATION REQUIREMENTS IN MAIN()
//void Evolva::setSolution(long a)
//{
//    std::string newSolution = std::to_string(a);
//    generationCount = 0;
//}


//---------------------------------------------------------------------------------------------
// HELPER METHODS

#include <stdio.h>
#include <stdlib.h>

// Evolve the population:
void Evolva::nextGeneration()
{
    // int i = 1 + (rand() % 6); /* random int between 1 and 6 */
    // double d = rand()/RAND_MAX; /* random double-float between 0 and 1 */
 
    // Evolve the population by one step until we reach an optimum solution
    if(myPopulation->getFittest()->getFitness() < Goals::getMaxFitness())
    {
        myPopulation = Evolution::evolvePopulation(myPopulation);  // WATCH OUT * * * < Check there is appropriate variation/selevtion for evolutionary purposes - FITTEST's FITNESS DOES NOT SEEM TO GROW THAT WELL!
        generationCount++;
        
//        std::string str = "Generation: " + std::to_string(generationCount)
//        +  " Fittest's fitness: " + std::to_string(myPopulation->getFittest()->getFitness())
//        + " Maximum possible fitness: " + std::to_string(Goals::getMaxFitness());
//        post(str.c_str());
        
        // Output inforamtion about the system
        outlet_anything(m_outlets[1], gensym(myPopulation->getFittest()->toString().c_str()), 0, NULL);
        outlet_int(m_outlets[2], generationCount);  // current generation
        outlet_int(m_outlets[3], myPopulation->getFittest()->getFitness()); // fittest's fitness value
        outlet_int(m_outlets[4], Goals::getMaxFitness()); // maximum fitness value
        outlet_int(m_outlets[5], 0);  // flag out that we have not reached an optimal fitness
    }
    else
    {
        // Depending on your input, the result will be constant once the solution has been reached
        outlet_int(m_outlets[5], 1);  // flag out that we reached an optimal fitness
        
        // post("Solution found!");
        //			post( ("Generation: " + std::to_string(generationCount)).c_str() );
        //          post(("Genes: " + myPopulation->getFittest()->toString()).c_str());
    }
    
}
 //
void Evolva::express(){
    // Set list of notes to play
    notesToPlay = chooseNotes(myPopulation->getFittest()->toString(), notesPerUpdate);
}

// Output in a new thread
void Evolva::output(){
    
    // Play notes in a NEW THREAD:
    mcppthread_create(this, &thread); // pass object where to find appropriate run() function AND a thread ID address
        // -> systhread_create( (method) threaded_func, this, 0, 0, 0, &thread );
}


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

// INTEGER TO CHAR* strings USING AN INPUT c-style char array AS A BUFFER
char* Evolva::myItoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }
    
    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;
    
    // Lookup character based on their numerical value
    // 0 is the 35th, negative values grow to the left, positive to the right
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );
    
    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    
    // Add string termination character
    *ptr-- = '\0';
    
    // Flip string around
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

//---------------------------------------------------------------------------------------------


// INLET/OUTLET ASSIST POPUP

void Evolva::assist(void* b, long m, long a, char* s) { //template function that gets triggered when "ASSIST_INLET"
    //is invoked by Max
    //param "a" identify the inlet/outlet
    if (m == ASSIST_INLET) { //inlet
        switch (a) {
            case 0: sprintf(s,
                            "\n<bang> evolve population and output notes \n'setSolution' + <string> : trigger change to current GA's solution using colour-names IDs 'red' 'green' 'blue' 'yellow' \n<int> : set n# of notes output at each update");
                break;
            case 1: sprintf(s, "\n<bang> evolve population \n<int> : set ms between output notes");
                break;
            case 2: sprintf(s, "\n<bang> choose notes to play and load output vector");
                break;
            case 3: sprintf(s, "\n<bang> choose notes to play and output them \n<int> : set ms between output notes");
                break;
        }
    }
    else {	// outlet
        switch (a) {
            case 0 : sprintf(s, "\nMIDI notes out");
                break;
            case 1 : sprintf(s, "\n--fittest's genotype--");
                break;
            case 2 : sprintf(s, "\n--current generation count--");
                break;
            case 3 : sprintf(s, "\n--fittest individual's fitness value--");
                break;
            case 4 : sprintf(s, "\n--maximum fitness value--");
                break;
            case 5 : sprintf(s, "\n--flag: solution found--");
                break;
        }
    }
}


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

