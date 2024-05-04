#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <ctime>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <random>
#include "split.h"
int main(int argc, char *argv[]) {
    int distributionMembers = readFromFile("distributersCommitteeMemberCount=");
    int workerNumber = atoi(argv[1]);
    int workerBags = readFromFile("wheatBagsCount=");

    key_t shm_key = ftok("/tmp", 'S');
    key_t shm_keyStorage = ftok("/tmp", 'G');
    key_t shm_keyDistribute = ftok("/tmp", 'd'); // Using a constant value as the second argument for uniqueness

    int shmid_distribution = shmget(shm_keyDistribute, sizeof(struct distribution), IPC_CREAT | 0666);
    if (shmid_distribution == -1) {
        perror("shmget (dist)");
        return 1;
    }
    struct distribution *distributers_ptr = (struct distribution *)shmat(shmid_distribution, NULL, 0);
    if (distributers_ptr == (void *)-1) {
        perror("shmat (dist)");
        return 1;
    }

    int shmid_storage= shmget(shm_keyStorage, sizeof(struct storageRoom), IPC_CREAT | 0666);
    if (shmid_storage == -1) {
        perror("shmget (storage)");
        return 1;
    }
    struct storageRoom *storage = (struct storageRoom *)shmat(shmid_storage, NULL, 0);
    if (storage == (void *)-1) {
        perror("shmat (storage)");
        return 1;
    }

    int shmid_splitting = shmget(shm_key, sizeof(struct splitting), IPC_CREAT | 0666);
    if (shmid_splitting == -1) {
        perror("shmget (dist)");
        return 1;
    }
    struct splitting *splitting_ptr = (struct splitting *)shmat(shmid_splitting, NULL, 0);
    if (splitting_ptr == (void *)-1) {
        perror("shmat (dist)");
        return 1;
    }
    splitting_ptr->pids[workerNumber] = getpid();

    int semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = 1;

    while(1){
        semop(semid, &lock, 1);
        if(storage->totalWeight > 0){
            for(int i=0; i< distributionMembers; i++){
                if(distributers_ptr->dist[i].numberOfBags == 0 && distributers_ptr->dist[i].status==0 && storage->totalWeight > 0){
                    if((storage->totalWeight - workerBags) < 0){
                        distributers_ptr->dist[i].numberOfBags = storage->totalWeight ;  
                        storage->totalWeight = 0;            
                    }
                    else{
                        distributers_ptr->dist[i].numberOfBags = workerBags;
                        storage->totalWeight -= workerBags;    
                    }   
                }
            }
        }
        semop(semid, &unlock, 1);     
    }
    if (shmdt(splitting_ptr) == -1) {
        perror("shmdt (dist)");
        return 1;
    }

    return 0;
}