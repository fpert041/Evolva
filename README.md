EVOLVA
============

## A MIDI note generator for Max-MSP based on a Genetic Algorithm

### STATUS:


#### Version: 0.3 (alpha)

Structure: 
 - main() cpp calls object's class-wrapper (Evolva hpp+cpp) and registers the external's interface functions for Max.

 - The 'Evolva' class calls up the main algorithmic structure (Evolution) and the population-defining classes (Population <- Individual). 
 - "FitnessFuncs" hpp+cpp contain the global methods/variables that set the target solution ("Goal" namespace)


### WIP/TODO: 

- At the moment it compiles and runs in Max. It outputs MIDI notes based on an simple mapping of the evolving fittest individual's genetic code

- Notes on things to look at:
    * The multithreading system does not use mutex locks. It seems stable enough but... is it? Max crashes if I add mutexes (I placed them as comments in the code to remind myself)
    * I would like to make the musical mapping of the genotype easierr, more complex and more flexible.

- Mutex possible issue in "Evolva::threaded_function(...)":

```
int notesPlayed = 0;
//<<<<<<<<<<<<<<<< { *BUG IN MUTEX? }
/////systhread_mutex_lock(t_mutex); //make sure that other threads cannot change critical variables whilst outputting
if(!notesToPlay.empty()){ // make sure "notes to play" IS_NOT an empty list of notes
    while(notesPlayed < notesPerUpdate){
        outlet_int(m_outlets[0], notesToPlay[notesPlayed] + 24);
        (...)
    }
}
////systhread_mutex_unlock(t_mutex);  //<<<<<<<<<<<<<<<< { *BUG IN MUTEX? }
systhread_exit(0);
```

- In "Evolva::bang(...)": (can the evolutionary rules be improved?)

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


## WARNING: WIP!!!


### Copyright:

Francesco Perticarari (c) 2017
