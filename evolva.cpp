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

// inherit from the MSP base class and a thread-building friendly template class:

class Evolva : public MaxCpp<Evolva>, ThreadingHelp<Evolva>{
public:
    
    // 'Target' set of notes to play, a bit string representing the state of 48 notes
    std::string solutionString = "000000000000100000000000101010110101100000000000";
    // The set of notes to start out with
    std::string originalPop ="000000000000100000000000000000000000101010010011";
    int generationCount;
    //Population myPop;  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Need just one note per 'bang' fow now
    int notesPerUpdate = 1;
    // Initialize variable to contain new solution during transitions
    std::string newSol="000000000000000000000000000000000000101010010011";
    // Set how many milliseconds between notes
    volatile float bangInterval = 500;

	Evolva(t_symbol * sym, long ac, t_atom * av) {
		setupIO(2, 2);
        
        //Initialize genetic related variables <<<<<<<<<<<<<<<<<<<<<<<<<<<<
        ///FitnessCalc.setSolution(solutionString);
        ///myPop = new Population(100, originalPop);
        
#ifndef RANDOM_SEED
#define RANDOM_SEED
        static const int seed = time(NULL); /* random seed global variable (make sure it is only defined once */
#endif
        srand(seed); /* seed random number generator */
        
		post("object created");
	}
	
	~Evolva() {
		post("object freed");
	}
    
    
    // Get a list of notes to play
    /*****
     final List<Integer> notesToPlay = selectNotes(myPop.getFittest().toString(), notesPerUpdate);
     ******/
    
    /*****************************************THREADING TRIAL start snippet*********************************************/

    // threadable function
    void thread_function(int notesPerUpdate, int bangInterval)
    {
        //				long startTime = System.currentTimeMillis();
        int notesPlayed = 0;
        while(notesPlayed < notesPerUpdate){
            ///outlet(0, notesToPlay.get(notesPlayed) + 24);  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
            ///long ac = 200;
            ///t_atom * av;
            ///outlet_anything(m_outlets[0], gensym("test"), ac, av);
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
    
    t_systhread thread; // thread ID

    /* Create worker thread: this function gets called whenever a thread is created */
    const void run(){
        thread_function(notesPerUpdate, bangInterval);
        
        post("flag: circular threading works %i",notesPerUpdate);
    }
    
    
    /*****************************************THREADING TRIAL end snippet*********************************************/
    
	// interface  methods:
    
	void bang(long inlet) {
        
        int i = 1 + (rand() % 6); /* random int between [1 and 6] */
        double d = rand()/RAND_MAX; /* random double-float between [0 and 1[ */
        
        
        // Evolution the population by one step until we reach an optimum solution
        ///if(myPop.getFittest().getFitness() < FitnessCalc.getMaxFitness())
        if(TRUE) //<<TODO ---^
        {
            ///myPop = Algorithm.evolvePopulation(myPop);
            generationCount++;
            std::cout << "" /*("Generation: " + generationCount + " Fittest: " + myPop.getFittest().getFitness())*/ << std::endl;
        }
        else
        {
            // Depending on your input, the result will be constant once the solution has been reached
            std::cout << "Solution found!" << std::endl;
            //			System.out.println("Generation: " + generationCount);
            //			System.out.println("Genes:");
            //			System.out.println(myPop.getFittest());
        }
        
        const std::vector<int> notesToPlay = chooseNotes("myPop.getFittest().toString()", notesPerUpdate);
        
        // Play notes in a NEW THREAD:
        mcppthread_create(this, &thread); // pass object where to find appropriate run() function AND a thread ID address
                //systhread_create( (method) threaded_func, this, 0, 0, 0, &thread );
        
        
		post("bang in inlet %i!", inlet);
	}
    
    
    void inFloat(long inlet, double v) {
        post("inlet %ld float %f", inlet, v);
        
        bangInterval = v;
        
        outlet_float(m_outlets[0], v);
    }
    
    void inInt(long inlet, long v) {
        post("inlet %ld int %ld", inlet, v);
        
        bangInterval = v;
        
        outlet_int(m_outlets[0], v);
    }
    
	
    /* -- for MSP objects---
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
    
    // Change the solution of the GA
    void setSolution(std::string newSolution)
    {
        // Implement logic to change solution here
        
        if(newSolution.compare("red"))
        {
            newSol = "110010111001100000000000100000000000000000000000";
        }
        
        if(newSolution.compare("blue"))
        {
            newSol = "000000000000101101011010100000000000000000000000";
        }
        
        if(newSolution.compare("green"))
        {
            newSol = "000000000000000000000000101010110101100000000000";
        }
        if(newSolution.compare("yellow"))
        {
            newSol = "000000000000000000000000000000000000101011010101";
        }
        
        // Set new fitness string  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        ///////FitnessCalc.setSolution(newSol);
        // Create a new population with previous string
        //////myPop = new Population(100, solutionString);
        // Replace old solution string with new one
        solutionString = newSol;
        // Reset generation count
        generationCount = 0;
    }
    
    // TODO: overload the 'setSolution' function to react differently to different data types
    /*
    void setSolution(int a)
    {
      std::string ab = Long.toBinaryString(a);
      FitnessCalc.setSolution(ab);
      generationCount = 0;
    }
    */
    
    
    //---------------------------------------------------------------------------------------------
    // HELPER METHODS

private:
    
    std::vector<int> chooseNotes(std::string fittest, int howManyNotes){
        
        
        // do s**t...
        
        std::vector<int> notesToPlay = std::vector<int>();
        
        // more c**p...
        
        return notesToPlay;
    }
    
	
//	// optional method: gets called when the dsp chain is modified
//	// if not given, the MspCpp will use Example::perform by default
//	void dsp(t_object * dsp64, short *count, double samplerate, long maxvectorsize, long flags) { 
//		// specify a perform method manually:
//		REGISTER_PERFORM(Example, perform);
//	}
};




//MAIN() :

C74_EXPORT int main(void) {
    // create a class with the given name:
    Evolva::makeMaxClass("evolva");
    
    // register methods callers in Max
    REGISTER_METHOD(Evolva, bang);
    REGISTER_METHOD_LONG(Evolva, inInt);
    REGISTER_METHOD_FLOAT(Evolva, inFloat);
    
    // methods for handling float/int messages directly (no method name in Max):
    REGISTER_INLET_LONG(Evolva, inInt);
    REGISTER_INLET_FLOAT(Evolva, inFloat);
}
