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
#include "dist.h"

int main(int argc, char *argv[]) {
    seedRandom();
    int familyCounter=0, workerCounter=0;
    int distributionTeams = readFromFile("distributersCommitteesCount=");
    int distributionMembers = readFromFile("distributersCommitteeMemberCount=");
    int minimumEnergy = readFromFile("minimumEnergy=");
    int maximumEnergy = readFromFile("maximumEnergy=");  
    int energyDepletionRate = readFromFile("energyDepletionRate=");  
    int familiesStarvationRate = readFromFile("familiesStarvationRate=");
    int timePerStarvationRate = readFromFile("timePerStarvationRate=");
    int families= readFromFile("families=");
    int teamleadNumber = atoi(argv[1]);
    int processPerTeamPlusLead =distributionMembers;

    cout << "Team lead with process id " << getpid() <<" with number " << teamleadNumber << endl;

    key_t shm_key = ftok("/tmp", 'd'); // Using a constant value as the second argument for uniqueness
    key_t shm_keyFamily = ftok("/tmp", 'f'); // Using a constant value as the second argument for uniqueness

    int shmid_distribution = shmget(shm_key, sizeof(struct distribution), IPC_CREAT | 0666);
    if (shmid_distribution == -1) {
        perror("shmget (dist)");
        return 1;
    }
    struct distribution *distributers_ptr = (struct distribution *)shmat(shmid_distribution, NULL, 0);
    if (distributers_ptr == (void *)-1) {
        perror("shmat (dist)");
        return 1;
    }
    int shmid_families = shmget(shm_keyFamily, sizeof(struct families), IPC_CREAT | 0666);
    if (shmid_families  == -1) {
        perror("shmget (families)");
        return 1;
    }
    struct families *families_ptr = (struct families *)shmat(shmid_families, NULL, 0);
    if (families_ptr == (void *)-1) {
        perror("shmat (families)");
        return 1;
    }
    distributers_ptr->pids[teamleadNumber] = getpid();
    distributers_ptr->dist[teamleadNumber].pidTeam = getpid();
    distributers_ptr->dist[teamleadNumber].energy = getRandomRange(minimumEnergy, maximumEnergy);
    distributers_ptr->dist[teamleadNumber].status=0;

    // for(int i=1; i< processPerTeamPlusLead; i++){
    //     if(!fork()){
    //         distributers_ptr->pids[(teamleadNumber * processPerTeamPlusLead)+i] = getpid();
    //         distributers_ptr->dist[teamleadNumber].pidTeam[i] = getpid();
    //         distributers_ptr->dist[teamleadNumber].energy[i] = getRandomRange(minimumEnergy, maximumEnergy);
    //         exit(0);
    //     }
    // }

    if(teamleadNumber==0){
        if(!fork()){
            while(1){
                for(int i=0; i< families; i++){
                    cout << "Starvation rate for Family with id :" << families_ptr->fam[i].id << " is " << families_ptr->fam[i].starvationRate << endl;
                }
                if(checkFamilies(families_ptr, families)){
                    cout << "All families are dead" << endl;
                    exit(0);
                }
                sleep(3);
            }
        }
        if(!fork()){
            while(1){
                for(int i=0; i< families; i++){
                    if(families_ptr->fam[i].status!=2){
                        families_ptr->fam[i].starvationRate+=familiesStarvationRate;
                    }
                }
                if(checkFamilies(families_ptr, families)){
                    exit(0);
                }
                sleep(timePerStarvationRate);
            }       
        }
    }
    int semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = 1;


    while(1){
        sleep(2);
        if(distributers_ptr->dist[teamleadNumber].status==0 ){
            for(int i=0; i<families; i++){
                if (families_ptr->fam[i].status == 0 && distributers_ptr->dist[teamleadNumber].numberOfBags > 0 && i == findMaxStarvationIndex(families_ptr,families)){  
                    families_ptr->fam[i].status =1;      
                    distributers_ptr->dist[teamleadNumber].status=1;  
                    cout << "Dist team " << teamleadNumber << " Going to family with id " << families_ptr->fam[i].id << 
                    " With bags " << distributers_ptr->dist[teamleadNumber].numberOfBags << endl;
                    if(families_ptr->fam[i].starvationRate - (distributers_ptr->dist[teamleadNumber].numberOfBags * 10) > 0){
                       families_ptr->fam[i].starvationRate -=(distributers_ptr->dist[teamleadNumber].numberOfBags * 10);
                    }
                    else{
                        families_ptr->fam[i].starvationRate=0;
                    }
                    distributers_ptr->dist[teamleadNumber].energy-= energyDepletionRate;
                    cout << " Team " << teamleadNumber << " sleeping " << energyDelay(distributers_ptr->dist[teamleadNumber].energy) << endl;
                    distributers_ptr->dist[teamleadNumber].numberOfBags = 0;
                    sleep(energyDelay(distributers_ptr->dist[teamleadNumber].energy));
                    cout << "Dist team " << teamleadNumber << " Gave wheat to family with id " << families_ptr->fam[i].id<< ", going back " << endl; 
                    families_ptr->fam[i].status =0;   
                    distributers_ptr->dist[teamleadNumber].status=0; 
                    distributers_ptr->dist[teamleadNumber].energy-= energyDepletionRate;
                    sleep(energyDelay(distributers_ptr->dist[teamleadNumber].energy));
                }
                if(families_ptr->fam[i].starvationRate>=90 && families_ptr->fam[i].status!=2){
                    families_ptr->fam[i].status=2;
                    cout << "Family with id " << families_ptr->fam[i].id << " is dead" << endl;
                }
            }
        }
        // if(checkFamilies(families_ptr, families)){
        //     break;
        // }
    }
    if (shmdt(distributers_ptr) == -1) {
        perror("shmdt (dist)");
        return 1;
    }
    if (shmdt(families_ptr) == -1) {
        perror("shmdt (families)");
        return 1;
    }

    return 0;
}