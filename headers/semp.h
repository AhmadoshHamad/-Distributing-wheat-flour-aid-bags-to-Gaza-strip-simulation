#ifndef SEMP_H
#define SEMP_H


sem_t * create_openSemaphore( char * sem_name){
    sem_t * mutex = sem_open(sem_name, O_CREAT, 0666, 1);
            if (mutex == SEM_FAILED) {
                perror("sem_open");
                exit(EXIT_FAILURE);
            }

    // cout << "created ! \n";
    return mutex;

}

int  PlaneCriticalSection(int key){
        // create a shared memory if exisits use it 
        char * shmPtr = attachSharedMemory( create_OpenSharedMemory(key));

        // return a pointer of the shared memory to use it 
   
        int readed;

        memcpy(&readed,shmPtr,sizeof(int));
        int retVal = readed;   
        cout << "Produced value is " << readed << endl;
        readed++;
        memcpy(shmPtr,&readed,sizeof(int));
    
        
    
    cout << "Produced\n";
    return retVal;

}


void PlaneCriticalSection2(int key){
        // create a shared memory if exisits use it 
        char * shmPtr = attachSharedMemory( create_OpenSharedMemory(key));

        // return a pointer of the shared memory to use it 
   
        int readed;

        memcpy(&readed,shmPtr,sizeof(int));
        cout << "Produced value is " << readed << endl;
        readed++;
        memcpy(shmPtr,&readed,sizeof(int));
    
        
    
    cout << "Produced\n";

}


#endif