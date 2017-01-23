//
//  Evolvaa.hpp
//  evolva
//
//  Created by Francesco Perticarari on 22/01/2017.
//
//

#ifndef Evolvaa_hpp
#define Evolvaa_hpp

#include "maxcpp.h"

#include "ext_systhread.h"
#include "helpingThreads.h"
//#include <thread> //useless in Max - it makes the program crash

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>

#include "FitnessFuncs.hpp"
#include "Evolution.hpp"
#include "Individual.hpp"
#include "Population.hpp"


class Evolva : public MaxCpp<Evolva>, ThreadingHelp<Evolva>{
public:
    
    Evolva(t_symbol * sym, long ac, t_atom * av);
    ~Evolva();
    
    // 'Target' set of notes to play, a bit string representing the state of 48 notes
    std::string solutionString = "000000000000100000000000101010110101100000000000";
    // The set of notes to start out with
    std::string originalPop ="000000000000100000000000000000000000101010010011";
    int generationCount;
    
    // A pointer-variable for our population
    std::shared_ptr<Population> myPopulation;
    std::vector<int> notesToPlay;
    
    // Need just one note per 'bang' fow now
    int notesPerUpdate = 1;
    // Initialize variable to contain new solution during transitions
    std::string newSol="000000000000000000000000000000000000101010010011";
    // Set how many milliseconds between notes
    volatile float bangInterval = 500;
    
    void thread_function(int notesPerUpdate, int bangInterval, std::vector<int>&);
    
    const void run();
    
    const void quit();
    
    
    void bang(long inlet);
    
    void setSolution(std::string);
    void setSolution(long);
    
    void inFloat(long inlet, double v);
    
    void inInt(long inlet, long v);
    
private:
    
    // Select and return a list of notes
    std::vector<int> chooseNotes(std::string, int); // CORE: "MUSICAL" DECISION MAKING METHOD
    
    //	// optional method: gets called when the dsp chain is modified
    //	// if not given, the MspCpp will use Example::perform by default
    //	void dsp(t_object * dsp64, short *count, double samplerate, long maxvectorsize, long flags) {
    //		// specify a perform method manually:
    //		REGISTER_PERFORM(Example, perform);
    //	}
    
};

#endif /* Evolvaa_hpp */