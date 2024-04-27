#include "headers/all.h"
#include "headers/semp.h"
#define key 10

char* SEM_MUTEX_CONTAINER = "/containers";
int shmid; // shared memory id
char * shmPtr; // shared memory pointer

void* producer(void* arg) {
        sem_t * mutex;
        
char* SEM_MUTEX_COTAINER = "/containers";
        mutex = create_openSemaphore(SEM_MUTEX_CONTAINER);

        sem_wait(mutex);
        // create a shared memory if exisits use it 
        char * shmPtr = attachSharedMemory( create_OpenSharedMemory(key));

        // return a pointer of the shared memory to use it 
   
    int readed;

    memcpy(&readed,shmPtr,sizeof(int));
    cout << "Produced value is " << readed << endl;
    readed++;
    memcpy(shmPtr,&readed,sizeof(int));
    
        
    
    cout << "Produced\n";
     

    sem_post(mutex);
    
    
    return NULL;
}

int main(){
    producer(NULL);

    
}