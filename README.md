EVOLVA
============

## A MIDI note generator for Max-MSP based on a Genetic Algorithm

### STATUS:

Structure in place: 
 - main() cpp calls object's class-wrapper (Evolva hpp+cpp) and registers the external's interface functions for Max.

 - The 'Evolva' class calls up the main algorithmic structure (Evolution) and the population-defining classes (Population <- Individual). 
 - "FitnessFuncs" hpp+cpp contain the global methods/variables that set the target solution ("Goal" namespace)

The external compiles, interfaces with Max and can launch a separate thread. There are at least two types of bugs that prevent me from making the external fully operational (see below). 

### ISSUES: 

- At the moment it compiles and runs in Max, but I am having troubles stabilizing the 2ndary thread (mutexes, safe termination etc):
    * If I implement the full threading system (mutex - safe), as I thought it may need to run, Max crashes
    * At present, I would like to integrate the comments where I placed // <<<<<<<< after the line - Note: they are commented out because make the program crash if uncommented

- In "Evolva::threaded_function(...)":

```
int notesPlayed = 0;
//<<<<<<<<<<<<<<<< { *BUG IN MUTEX! }
/////systhread_mutex_lock(t_mutex); //make sure that other threads cannot change critical variables whilst outputting
if(!notesToPlay.empty()){ // make sure "notes to play" IS_NOT an empty list of notes
    while(notesPlayed < notesPerUpdate){
        outlet_int(m_outlets[0], notesToPlay[notesPlayed] + 24);
        (...)
    }
}
////systhread_mutex_unlock(t_mutex);  //<<<<<<<<<<<<<<<< { *BUG IN MUTEX! }
systhread_exit(0);
```

- In "Evolva::bang(...)": (not a crash critical bug but the evolutionary rules may need tweaking)

```
// Evolution the population by one step until we reach an optimum solution
if(myPopulation->getFittest()->getFitness() < Goals::getMaxFitness())
{
    myPopulation = Evolution::evolvePopulation(myPopulation);  // WATCH OUT * * * < Check there is appropriate variation/selevtion for evolutionary purposes - FITTEST's FITNESS DOES NOT SEEM TO GROW THAT WELL!
    generationCount++;
    std::string str = "Generation: " + std::to_string(generationCount) 
    +  " Fittest: " + std::to_string(myPopulation->getFittest()->getFitness());
    post(str.c_str());
}
```


### TODO:

 - Make sure the threading system is safe (possibly exploring why mutex bloks are crashing max if included)
 - Make sure evolution works
 - Explore output's musical scope + possibly increase the number of note sent out at each bang 


## WARNING: WIP!!!


### Copyright:

Francesco Perticarari (c) 2017
