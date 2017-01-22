EVOLVA
============

## A MIDI note generator for Max-MSP based on a Genetic Algorithm

### STATUS:

Structure (more or less) in place: A main cpp file wrapping the external and containing main calling up the main algorithm structure (Evolution) and the population defining classes (Population <- Individual). "FitnessFuncs" hpp+cpp contain some global methods/variables I think I'll need - they may end up being incorporated in one of the other objects later on.

### TODO: Define all methods and constructors so that the main evolva wrapper class can spawn a population, make the individuals "fight" to prove their fitness and end up with a good enough solution to generate a sequence of notes - A separate thread (systhread) should deal with the output of the those notes

## WARNING: WIP!!!


### Copyright:

Francesco Perticarari (c) 2017
