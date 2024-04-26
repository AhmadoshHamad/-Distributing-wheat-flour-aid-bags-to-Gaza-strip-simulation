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

// Header files
#include "headers/plane.h"
#include "headers/SharedMemory.h"
#include "headers/Container.h"

using namespace std;

#define MEMORY_SIZE  4096
#define key 10


int shmid; // shared memory id
char * shmPtr; // shared memory pointer


int main(int argc, char * argv []){
    
        // create a shared memory if exisits use it 
    if ( (shmid = shmget(key, MEMORY_SIZE, IPC_CREAT | 0666)) < 0 ) {
        perror("shmget fail");
        exit(1);
    }

        // return a pointer of the shared memory to use it 
    if ( (shmPtr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
        perror("shmat: parent");
        exit(2);
    }

    Plane plane(5,50,2,12);
   

    Container container(plane.altidute,100);

    memcpy(shmPtr,&container,sizeof(Container));
    
   

    


}