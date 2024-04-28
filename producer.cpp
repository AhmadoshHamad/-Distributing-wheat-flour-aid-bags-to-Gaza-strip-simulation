#include "headers/all.h"
#include "headers/semp.h"
#define key 10

char SEM_MUTEX_CONTAINER[] = "/containers";
int shmid; // shared memory id
char * shmPtr; // shared memory pointer

void* producer(void* arg) {
        sem_t * mutex;
        
        mutex = create_openSemaphore(SEM_MUTEX_CONTAINER);

        sem_wait(mutex);

        PlaneCriticalSection2(10);

         sem_post(mutex);
    
    
    return NULL;
}

int main(){
    producer(NULL);

    
}