    #include <bits/stdc++.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <semaphore.h>
    #include <iostream>

// Header files
#include "plane.h"
#include "SharedMemory.h"
#include "Container.h"
#include "semp.h"

using namespace std;

#define MEMORY_SIZE  4096