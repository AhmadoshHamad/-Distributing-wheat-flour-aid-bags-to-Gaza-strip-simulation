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


#include "headers/all.h"
int shmid;
char * shmPtr;
// char * collect_sem = "/collectSemapore";
//sem_t *sem;

#define SEM_KEY 20 // Example key for the semaphore
struct sembuf lock2 = {0, -1, SEM_UNDO}; // Semaphore lock operation
struct sembuf unlock2 = {0, 1, SEM_UNDO}; // Semaphore unlock operation

// int readFromFile(std::string);

using namespace std;

struct colleCommittee{
    int pidTeam[5];
    int energy[5];
    int bag ; // the wheat flour containers

};
struct collectors {
    pid_t pids[30]; // Array to store process IDs
    struct colleCommittee colle[20];
};


 
struct safeStorage{
    int wheatFlour;
};
int  collectCriticalSection(int);

void readContainer(int i ,struct collectors *collectors_ptr);


int main(int argc, char *argv[]) {
    seedRandom();
    int collectorsTeams = readFromFile("collectorsCommitteesCount=");
    int collectorsMembers = readFromFile("collectorsCommitteeMemberCount=");
    int minimumEnergy = readFromFile("minimumEnergy=");
    int maximumEnergy = readFromFile("maximumEnergy=");  
    int teamleadNumber = atoi(argv[1]);
    int processPerTeamPlusLead =collectorsMembers;

    key_t shm_keyCollectors = ftok("/tmp", 'A'); // Using a constant value as the second argument for uniqueness
    key_t shm_keyStorage = ftok("/tmp", 'G');

    // Get the ID of the existing shared memory segment created by main.cpp
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

     // Get the ID of the existing shared memory segment created by main.cpp
    int shmid_safeStorage = shmget(shm_keyStorage, sizeof(struct safeStorage), IPC_CREAT | 0666);
    if (shmid_collectors == -1) {
        perror("shmget (collectors)");
        return 1;
    }

    // Attach to the shared memory segment <--- --->
    struct safeStorage *safeStorage_ptr = (struct safeStorage *)shmat(shmid_safeStorage, NULL, 0);
    if (safeStorage_ptr == (void *)-1) {
        perror("shmat (collectors)");
        return 1;
    }

    collectors_ptr->pids[teamleadNumber * processPerTeamPlusLead] = getpid();
    collectors_ptr->colle[teamleadNumber].pidTeam[0] = getpid();
    collectors_ptr->colle[teamleadNumber].energy[0] = getRandomRange(minimumEnergy, maximumEnergy);

    for(int i=1; i< processPerTeamPlusLead; i++){
        if(!fork()){
             collectors_ptr->pids[(teamleadNumber * processPerTeamPlusLead)+i] = getpid();
             collectors_ptr->colle[teamleadNumber].pidTeam[i] = getpid();
             collectors_ptr->colle[teamleadNumber].energy[i] = getRandomRange(minimumEnergy, maximumEnergy);
            exit(0);
        }
    }
   int semid = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1) {
        perror("semget");
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;
    arg.val = 1;

    if(teamleadNumber ==0){
        safeStorage_ptr->wheatFlour = 0;
    }

    if (shmdt(collectors_ptr) == -1) {
        perror("shmdt (collectors)");
        return 1;
    }

        // get the continrer from plane 

        // shmid = create_OpenSharedMemory(10);
        // shmPtr = attachSharedMemory(shmid);
        shmid = create_OpenSharedMemory(15);
        shmPtr = attachSharedMemory(shmid);
   
        //read the shmem 
    // if (teamleadNumber== 0 ){
    // for (size_t i = 0; i < 5; i++){
    //   Container container(0,0,0);
    //   memcpy(&container,shmPtr,sizeof(Container));
    //   container.printDetails();
    //   shmPtr += sizeof(Container);
    // }
    //     }
    
   
    
    while(1){ 
        usleep(500000);
        semop(semid, &lock2, 1);
        int offset=collectCriticalSection(15); 
        cout << "offset " << offset << endl;
        shmPtr += 12;
        shmPtr += (offset * sizeof(Container));  
        semop(semid, &unlock2, 1);  
        Container container(0,0,0);
        while (1){
            /* code */
            memcpy(&container,shmPtr,sizeof(Container));
            cout << "!!!!container Details when collecting !!!!!!!!\n";
            //  container.printDetails();
            
            if (container.weight >0 && container.status == 2){
                collectors_ptr->colle[teamleadNumber].bag=container.weight;
                cout <<"amrooooooooooooooooooooooooo "<<container.weight<< endl ;
                cout <<"my id "<< teamleadNumber << " and my bag have "<<collectors_ptr->colle[teamleadNumber].bag<< endl ;
            // container.status =-1 ;
                //container.printDetails();
                safeStorage_ptr->wheatFlour += collectors_ptr->colle[teamleadNumber].bag;
                cout << "weight storage" << safeStorage_ptr->wheatFlour << endl;
                shmPtr = attachSharedMemory(shmid) ;
                break;
        }
            if(container.status == BLOWN)
                break;

            sleep(2);
         }
     
    
    sleep(2);
    }
    
       
    return 0;
}
// int readFromFile(std::string parameter) {
//     std::string value = "";
//     std::ifstream inputFile("vars.txt"); // Replace "vars.txt" with your file name
//     if (!inputFile) {
//         std::cerr << "Error opening file." << std::endl;
//         return -1; // Return a default value indicating error
//     }

//     std::string line;
//     while (std::getline(inputFile, line)) {
//         size_t equalPos = line.find(parameter);

//         if (equalPos != std::string::npos) { // If parameter found
//             // Extract the substring after the parameter
//             value = line.substr(equalPos + parameter.length());

//             // Print for debugging
//             // std::cout << "Parameter: " << parameter << ", Extracted value: " << value << std::endl;

//             // Convert the value string to an integer
//             inputFile.close(); // Close the file before returning
//             try {
//                 return std::stoi(value);
//             } catch (const std::invalid_argument& e) {
//                 std::cerr << "Invalid argument: " << e.what() << std::endl;
//                 return -1;
//             } catch (const std::out_of_range& e) {
//                 std::cerr << "Out of range: " << e.what() << std::endl;
//                 return -1;
//             }
//         }
//     }
//     inputFile.close(); // Close the file

//     std::cerr << "Parameter not found in the input file." << std::endl;
//     return -1; // Return a default value indicating error
// }


int  collectCriticalSection(int key){
        // create a shared memory if exisits use it 
        char * shmPtr = attachSharedMemory( create_OpenSharedMemory(key));
        shmPtr+=8;
        // return a pointer of the shared memory to use it 
   
        int readed;

        memcpy(&readed,shmPtr,sizeof(int));
        int retVal = readed;   
        cout << "Readed value is " << readed << endl;
        readed++;
        memcpy(shmPtr,&readed,sizeof(int));
    
        
    
    cout << "Produced\n";
    return retVal ;
}
/*
 semop(semid, &lock, 1);
        if(storage->totalWeight > 0){
            for(int i=0; i< distributionMembers; i++){
                if(distributers_ptr->dist[i].numberOfBags == 0 && distributers_ptr->dist[i].status==0 && storage->totalWeight > 0){

                    distributers_ptr->dist[i].numberOfBags = workerBags;
                    if((storage->totalWeight - workerBags) <= 0){
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
        semop(semid, &lock, 1);  
*/