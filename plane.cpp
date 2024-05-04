#include "headers/all.h"

0002 >> 0000000 >> 0000000 >> 0000000 


int shmid; // shared memory id
char * shmPtr; // shared memory pointer
char * plane_sem = "/planeSemapore";


int main(int argc, char * argv []){
    sem_t * mutex;  
    mutex =  create_openSemaphore(plane_sem);
    sem_wait(mutex);
      // create a shared memory if exisits use it 
    shmid = create_OpenSharedMemory(10);
    // return a pointer of the shared memory to use it 
    shmPtr = attachSharedMemory(shmid);

    // navigate to the shared memory location 
    int offset = PlaneCriticalSection(10);
    shmPtr += 4;
    shmPtr += (offset * sizeof(Container));

    // data to write
    int a = (argc > 1) ? atoi(argv[1]):999;
    Plane plane(5,50,2,150);
    Container container(plane.altidute + a,a +10);
    
    // copy data to shared memory
    memcpy(shmPtr,&container,sizeof(Container));

    sleep(1);
    // release the mutex lock
    sem_post(mutex);
}