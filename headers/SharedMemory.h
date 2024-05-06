#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H


#define MEMORY_SIZE  4096
#define KEY 10


using namespace std;

    

     int create_OpenSharedMemory(int key ){ // create a shared memory if exisits use it 
        int shmid; // shared memory id
        if ( (shmid = shmget(key, MEMORY_SIZE, IPC_CREAT | 0666)) < 0 ) {
            perror("shmget fail");
            exit(1);
        }
        //cout << "hello  " << shmid << endl;
        return shmid;
    }


     char* attachSharedMemory(int shmid){  // return a pointer of the shared memory to use it 
        
        char * shmPtr; // shared memory pointer 
        if ( (shmPtr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
            perror("shmat: parent");
            exit(2);
        }
        // cout << "attached ";
        return shmPtr;
    }

    void resetSharedMemory(){ // remove any previous data
        char zeros [4000];
        memset(zeros,0,4000);

        char * shmPtr = attachSharedMemory(create_OpenSharedMemory(10));
        memcpy(shmPtr,&zeros,sizeof(zeros));
    }


    

#endif