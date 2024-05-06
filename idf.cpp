#include "headers/all.h"

int shmid;
char * shmPtr;

#define IDFKEY 11
char sem_name[20] = "/idfsem";

Container container(0,0,0);

int main(int argc, char * argv []){

    sem_t * sem = create_openSemaphore(sem_name);

    while (1) {
        // Lock semaphore
        usleep(50000);
        sem_wait(sem);
        shmid = create_OpenSharedMemory(10); 
        shmPtr = attachSharedMemory(shmid);

        int offset = IdfCriticalSection(10);
        shmPtr += 8  ;
        shmPtr+= (offset * sizeof(Container));
        sem_post(sem);
       
        cout << "idf offset " << offset << endl;

        memcpy(&container,shmPtr,sizeof(Container));


    char  sharedSemName[20] = "/shared";
    char index[4]; // Convert integer index to string
    snprintf(index, sizeof(index), "%zu", offset);
    // create the semaphore between idf and container
    strcat(sharedSemName,index);
    cout << "sharedi  sem " <<sharedSemName << endl;
    


        sem_t * sharedSem = create_openSemaphore(sharedSemName); 
        // cout << "we need you !!!\n";
        while (1)  {
            usleep(50000);
            sem_wait(sharedSem);
            memcpy(&container,shmPtr,sizeof(Container));

            if(container.status == FALLING){
                container.damage();
                memcpy(shmPtr,&container,sizeof(Container));
            }
            else if(container.status == BLOWN)
                break;

            sem_post(sharedSem);
            
        
        }

        // container.printDetails();
    
    // exit(0);

    }
    
    
        // container.printDetails();
    
    



    //  if (sem_unlink(idf_semaphore) == -1) {
    //     perror("sem_unlink");
    //     exit(EXIT_FAILURE);
    // }


}