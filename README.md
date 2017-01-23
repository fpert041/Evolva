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
    * If I uncomment the methods that should produce the PHENOTYPE I have a full compilation but Max crashes (probably due to a pointer issue since it simply shuts down without an error log)
    * If I implement the full threading system as I thought it may need to run, Max crashes too
    * At present, the comments where I placed //<<<<<<<< after the line, are the ones I need but which make the program crash if uncommented
    * The issues are most probably under the hood, so I will have to follow these methods/instructions and see what actually causes the bug

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

- In "Evolva::bang(...)":

```
// Evolution the population by one step until we reach an optimum solution
if(/*myPopulation->getFittest()->getFitness()*/ 0 < Goals::getMaxFitness())  //<<<<<<<<<<<< { *BUG IN "getFitness" }
{
    myPopulation = Evolution::evolvePopulation(myPopulation);
    generationCount++;
    /*std::string str = "Generation: " + std::to_string(generationCount)  // { *BUG IN "getFitness" }
    +  " Fittest: " + std::to_string(myPopulation->getFittest()->getFitness()); //<<<<<<<<<<<<<<<<
    post(str.c_str());*/
}
```
...and...
```
// Set list of notes to play
//notesToPlay = chooseNotes(myPopulation->getFittest()->toString(), notesPerUpdate); //<<<<<<<<<<<<<<<< { *BUG IN THE FIRST PARAMETER! }
```

### TODO:

 - Fix the crash issues when invoking selection/fitness/population methods from the main Evolva class
 - Fix the crash issues due to threading mutex protection and safe shutting


## WARNING: WIP!!!


### Copyright:

Francesco Perticarari (c) 2017
