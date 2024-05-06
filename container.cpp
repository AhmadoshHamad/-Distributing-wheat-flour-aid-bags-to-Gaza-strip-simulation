#include "headers/all.h"

int shmid; // shared memory id 
char * shmPtr; // shared memory pointer
#define ContainerKEY 15
char container_semName[20] = "/container";
int maxWeight = readFromFile("ContainerWeight=");

int main(int argc , char * argv[]){ 
    cout << atoi(argv[1]) << endl;
        // crate or open semaphore
    sem_t * containerSem = create_openSemaphore(container_semName); 
        // lock the mutex 
    usleep(50000);
    sem_wait(containerSem); 
        // create a shared memory if exisits use it 
    shmid = create_OpenSharedMemory(ContainerKEY); 
        // return a pointer of the shared memory to use it 
    shmPtr = attachSharedMemory(shmid);
    Container container(atoi(argv[1])  , getRandomRange(50,maxWeight) ,  FALLING);
    container.printDetails(); 

        // navigate to the shared memory location 
    int offset = PlaneCriticalSection(ContainerKEY);
    shmPtr += 12;
    shmPtr += (offset * sizeof(Container));
        //write the container to the memory
    memcpy(shmPtr,&container,sizeof(Container)); 

        // release the mutex lock
    sem_post(containerSem);
    // cout << "offset " << offset << endl;

    char  sharedSemName[20] = "/shared";
    char index[4]; // Convert integer index to string
    snprintf(index, sizeof(index), "%zu", offset);
    // create the semaphore between idf and container
    strcat(sharedSemName,index);
    // cout << "sharedc sem " <<sharedSemName <<"hello" <<endl;
    


    sem_t * sharedSem = create_openSemaphore(sharedSemName);
   
    while (1){
        usleep(50000);
        sem_wait(sharedSem);

         memcpy(&container,shmPtr,sizeof(Container));
        
        if(container.status == BLOWN || container.status == LANDED){
            container.printDetails();
            break;
        }

        else if(container.status == FALLING){
            container.decreaseAltitude();
            memcpy(shmPtr,&container,sizeof(Container));
        }

        sem_post(sharedSem);
    }


}



