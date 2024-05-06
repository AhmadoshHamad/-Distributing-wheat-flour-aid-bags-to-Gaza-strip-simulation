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
struct family{
    int starvationRate;
    int status;
    int id;
};
struct families{
    struct family fam[50];
};
struct splitting{
    int pids[30];
};
struct colleCommittee{
    int pidTeam[5];
    int energy[5];
    int bag ; // the wheat flour containers

};
struct collectors {
    pid_t pids[30]; // Array to store process IDs
    struct colleCommittee colle[20];
};

void killSplitting(int processID){
    kill(processID, SIGKILL);
}
void killDistributingProcess(int processID,int i, int distributersDeathCount, struct splitting *splitting_ptr, int splitCounter){

   kill(processID, SIGKILL);
   cout << "Killed process " << processID  << endl;
   if(distributersDeathCount==0){
        killSplitting(splitting_ptr->pids[splitCounter]);
        if(!fork()){

            execlp("./dist", "dist", to_string(i).c_str(), nullptr);
        } 
   }

}
int shmid;
char * shmPtr;

#define ContainerKEY 15
char sem_name[20] = "/idfsem";

Container container(0,0,0);
void killCollector(int processID, int i){
   kill(processID, SIGKILL);
   if(!fork()){
        execlp("./collect", "collect", to_string(i).c_str(), nullptr);
   }
}
int main(int argc, char * argv []){
    int splitCounter=0;
    int distributers = readFromFile("distributersCommitteesCount=");
    int distributersDeathCount = readFromFile("distributersMinimumCount=");
    int collectors = readFromFile("collectorsCommitteesCount=");
    sem_t * sem = create_openSemaphore(sem_name);
    key_t shm_key = ftok("/tmp", 'd'); // Using a constant value as the second argument for uniqueness
    key_t shm_keyFamily = ftok("/tmp", 'f'); // Using a constant value as the second argument for uniqueness
    key_t shm_keyCollectors = ftok("/tmp", 'A'); // Using a constant value as the second argument for uniqueness

    int shmid_collectors = shmget(shm_keyCollectors, sizeof(struct collectors), IPC_CREAT | 0666);
    if (shmid_collectors == -1) {
        perror("shmget (collectors)");
        return 1;
    }

    // Attach to the shared memory segment
    struct collectors *collectors_ptr = (struct collectors *)shmat(shmid_collectors, NULL, 0);
    if (collectors_ptr == (void *)-1) {
        perror("shmat (collectors)");
        return 1;
    }
    int shmid_distribution = shmget(shm_key, sizeof(struct distribution), IPC_CREAT | 0666);
    if (shmid_distribution == -1) {
        perror("shmget (dist)");
        return 0;
    }

    struct distribution *distributers_ptr = (struct distribution *)shmat(shmid_distribution, NULL, 0);
    if (distributers_ptr == (void *)-1) {
        perror("shmat (dist)"); 
        return 0;
    }
    int shmid_families = shmget(shm_keyFamily, sizeof(struct families), IPC_CREAT | 0666);
    if (shmid_families  == -1) {
        perror("shmget (families)");
        return 1;
    }
    key_t shm_keySPlit = ftok("/tmp", 'S');
    struct families *families_ptr = (struct families *)shmat(shmid_families, NULL, 0);
    if (families_ptr == (void *)-1) {
        perror("shmat (families)");
        return 1;
    }
    int shmid_splitting = shmget(shm_keySPlit, sizeof(struct splitting), IPC_CREAT | 0666);
    if (shmid_splitting == -1) {
        perror("shmget (dist)");
        return 1;
    }
    struct splitting *splitting_ptr = (struct splitting *)shmat(shmid_splitting, NULL, 0);
    if (splitting_ptr == (void *)-1) {
        perror("shmat (dist)");
        return 1;
    }
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
        bool reserveNewSlot = false;
        // cout << "we need you !!!\n";
        while (1)  {
            for(int i=0; i< 4; i++){
                sem_wait(sharedSem);
                memcpy(&container,shmPtr,sizeof(Container));

                if(container.status == FALLING){
                    container.damage();
                    memcpy(shmPtr,&container,sizeof(Container));
                }
                else if(container.status == BLOWN){
                    reserveNewSlot = true;
                    sem_post(sharedSem);
                    break;
                }
                else if(container.status == LANDED){
                    reserveNewSlot = true;
                    sem_post(sharedSem);
                    break;
                }
                sem_post(sharedSem);
                sleep(1);
            }
            if(reserveNewSlot){
                reserveNewSlot=false;
                break;
            }
            for(int i=0; i<2;i++){
                for(int i=0; i<distributers;i++){
                    if(distributers_ptr->dist[i].energy<30 && distributers_ptr->dist[i].status==0){
                        if(families_ptr->fam[distributers_ptr->dist[i].status].starvationRate >=90){
                            families_ptr->fam[distributers_ptr->dist[i].status].status = 2;
                        }
                        else{
                            families_ptr->fam[distributers_ptr->dist[i].status].status = 0;
                        }
                        distributersDeathCount--;
                        cout << "Entered if statement" << endl;
                        distributers_ptr->dist[i].energy = 100;
                        distributers_ptr->dist[i].status = 0;
                        killDistributingProcess(distributers_ptr->dist[i].pidTeam, i, distributersDeathCount, splitting_ptr, splitCounter);
                        splitCounter++;
                    }
                }
                sleep(1);
            }
            for(int i=0; i<2; i++){
                for(int i=0; i<collectors;i++){
                    cout << "COLLERCTORS ENERGY" << collectors_ptr->colle[i].energy[0] << endl;
                    if(collectors_ptr->colle[i].energy[0]<30){
                        cout << "Killed collector " << endl;
                        killCollector(collectors_ptr->colle[i].pidTeam[0], i);
                        killSplitting(splitting_ptr->pids[splitCounter]);
                        splitCounter++;
                    }
                }
                sleep(1);
            }

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