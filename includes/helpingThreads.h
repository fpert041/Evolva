//
//  helpingThreads.h
//  evolva~
//
//  Created by Francesco Perticarari on 17/01/2017.
//
//

#ifndef helpingThreads_h
#define helpingThreads_h

/* This is our thread function.  It is like main(), but for a thread */

#include "ext_systhread.h"

template<typename T>
class ThreadingHelp {
public:

    static void *threaded_func(void *undefinedPtr){
        post("working thread flag");
        
        T *t = (T *) undefinedPtr;
        t->run();
        
        return NULL;
    }
    
    static long mcppthread_create( T* objectWithRun, t_systhread* t){
        return systhread_create( (method) threaded_func, objectWithRun, 0, 0, 0, t );
    }
    
    
    
    //----------------------------------------------------------------------
    // I tried to pass in a single member method as a pointer rather than an oblect to allow for multiple threads to be created with ease
    // But...
    // NOT WORKING -- I THINK IT IS DUE TO THE PASSED-IN FUNCTION POINTER GOING OUT OF SCOPE? 0_o /**/
    
    static const void *threaded_func2(void *undefinedPtr){
        post("working thread flag");
        
        void const (*pt2Function)(void) = NULL;
        pt2Function = (void const (*)(void)) undefinedPtr;
        pt2Function();
        
        return NULL;
    }
    
    /* NOT WORKING STUFF */
    static long mcppthread_create( const void (T::*myFunc)(void), t_systhread* t){
        return systhread_create( (method) threaded_func2, &myFunc, 0, 0, 0, t );
        // THIS IS POSIX: int pthread_create(pthread_t * pth, pthread_attr_t *att, void * (*function), void * arg);
        // THIS IS MAX SYSTHREAD: long systhread_create(method entryproc, void *arg, long stacksize, long priority, long flags, t_systhread *thread);
    }
    
    //overloaded version of the above that accepts static (global) functions
    static long mcppthread_create(void (* myFunc)(void), t_systhread* t){
        return systhread_create( (method) threaded_func2, &myFunc, 0, 0, 0, t );
        // THIS IS POSIX: int pthread_create(pthread_t * pth, pthread_attr_t *att, void * (*function), void * arg);
        // THIS IS MAX SYSTHREAD: long systhread_create(method entryproc, void *arg, long stacksize, long priority, long flags, t_systhread *thread);
    }

};

#endif /* helpingThreads_h */
