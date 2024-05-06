#include "headers/all.h"

int shmid; // shared memory id
char * shmPtr; // shared memory pointer
#define ContainerKEY 15
char container_semName[20] = "/container";


int main(int argc , char * argv[]){
        // crate or open semaphore
    sem_t * containerSem = create_openSemaphore(container_semName);
        // lock the mutex 
    usleep(50000);
    sem_wait(containerSem);
        // create a shared memory if exisits use it 
    shmid = create_OpenSharedMemory(10);
        // return a pointer of the shared memory to use it 
    shmPtr = attachSharedMemory(shmid);
    Container container(50,100,1);


        // navigate to the shared memory location 
    int offset = PlaneCriticalSection(10);
    shmPtr += 8;
    shmPtr += (offset * sizeof(Container));
        //write the container to the memory
    memcpy(shmPtr,&container,sizeof(Container));

        // release the mutex lock
    sem_post(containerSem);
    cout << "offset " << offset << endl;

    char  sharedSemName[20] = "/shared";
    char index[4]; // Convert integer index to string
    snprintf(index, sizeof(index), "%zu", offset);
    // create the semaphore between idf and container
    strcat(sharedSemName,index);
    cout << "sharedc sem " <<sharedSemName <<"hello" <<endl;
    

    cout << "before";
    sem_t * sharedSem = create_openSemaphore(sharedSemName);
    cout << "after";
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


        container.printDetails();
     
    // cout << "container  sem " <<container_sem << endl;
   

    // cout << argv[1] << endl;
    // Container container(atoi(argv[1]), 99,FALLING);
    // int decremental = 1;

    //     // copy data to shared memory
    // memcpy(shmPtr,&container,sizeof(Container));




    // cout << "Data written to shared memoery \n";
    // while (1){
    //     // sem_wait(container_mutex);
    //     memcpy(&container,shmPtr,sizeof(Container));
        
    //     if(container.status == BLOWN || container.status == LANDED){
    //         // sem_post(container_mutex);
    //         break;
    //     }
    //     if(container.status == FALLING){
    //         container.decreaseAltitude();
    //         memcpy(shmPtr,&container,sizeof(Container));
    //     }

    //     // usleep(500000);
    //     // sem_post(container_mutex);
    // }

    // cout << "container reached Ground or Blown!" << endl;
    // container.printDetails();
    


}




