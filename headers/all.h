    #include <bits/stdc++.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <cstring>
    #include <sys/stat.h>
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <sys/sem.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <ctime>
    #include <fstream>
    #include <random>
    #include <semaphore.h>


    #include <signal.h>

    // #include <semaphore.h>
    #include <iostream>

// Header files
#include "plane.h"
#include "SharedMemory.h"
#include "Container.h"
#include "semp.h"

using namespace std;


// struct sembuf lock = {0, -1, SEM_UNDO}; // Semaphore lock operation
// struct sembuf unlock = {0, 1, SEM_UNDO}; // Semaphore unlock operation

void reset(){
    system(" rm  /dev/shm/*");
}


#define MEMORY_SIZE  4096