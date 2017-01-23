#include "maxcpp.h"
#include "Evolva.hpp"

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
