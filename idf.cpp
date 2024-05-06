#include "headers/all.h"

struct distCommittee{
    int pidTeam;
    int energy;
    int status;
    int numberOfBags;
};
struct distribution {
    pid_t pids[50]; // Array to store process IDs
    struct distCommittee dist[50];
};

int shmid;
char * shmPtr;

#define ContainerKEY 15
char sem_name[20] = "/idfsem";

Container container(0,0,0);

int main(int argc, char * argv []){

    sem_t * sem = create_openSemaphore(sem_name);

    while (1) {
        // Lock semaphore
        usleep(50000);
        sem_wait(sem);
        shmid = create_OpenSharedMemory(ContainerKEY); 
        shmPtr = attachSharedMemory(shmid);

        int offset = IdfCriticalSection(ContainerKEY);
        shmPtr += 12  ;
        shmPtr+= (offset * sizeof(Container));
        sem_post(sem);
       
        // cout << "idf offset " << offset << endl;

        memcpy(&container,shmPtr,sizeof(Container));


        char  sharedSemName[20] = "/shared";
        char index[4]; // Convert integer index to string
        snprintf(index, sizeof(index), "%zu", offset);
        // create the semaphore between idf and container
        strcat(sharedSemName,index);
        // cout << "sharedi  sem " <<sharedSemName << endl;
    


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


void killDistributer(){
    key_t shm_key = ftok("/tmp", 'd'); // Using a constant value as the second argument for uniqueness

    int shmid_distribution = shmget(shm_key, sizeof(struct distribution), IPC_CREAT | 0666);
    if (shmid_distribution == -1) {
        perror("shmget (dist)");
        return ;
    }

    struct distribution *distributers_ptr = (struct distribution *)shmat(shmid_distribution, NULL, 0);
    if (distributers_ptr == (void *)-1) {
        perror("shmat (dist)"); 
        return ;
    }

}