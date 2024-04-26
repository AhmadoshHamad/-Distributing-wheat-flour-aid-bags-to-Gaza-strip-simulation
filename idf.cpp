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

// incldue plane class
#include "headers/plane.h"
#include "headers/SharedMemory.h"
#include "headers/Container.h"
#include "headers/Idf.h"

#define KEY 10
#define MEMORY_SIZE  4096


int main(){
    int shmid;
    char * shmPtr;
         // create a shared memory if exisits use it 
    if ( (shmid = shmget(KEY, MEMORY_SIZE, IPC_CREAT | 0666)) < 0 ) {
        perror("shmget fail");
        exit(1);
    }

        // return a pointer of the shared memory to use it 
    if ( (shmPtr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
        perror("shmat: parent");
        exit(2);
    }


   
    Container container(0,0);

    memcpy(&container,shmPtr,sizeof(Container));
    container.printDetails();




}