#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <fstream>
#include <random>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <wait.h>
#include "headers/SharedMemory.h"
using namespace std;
void resetSemaphores();
int planes=0;
int readFromFile(std::string);
struct colleCommittee{
    int pidTeam[5];
    int energy[5];
    int bag; // the wheat flour containers
};
struct collectors {
    pid_t pids[30]; // Array to store process IDs
    struct colleCommittee colle[20];
};

struct splitting {
    int pids[30];
};
struct distCommittee{
    int pidTeam;
    int energy;
    int status;
    int numberOfBags;
};
struct storageRoom{
    int totalWeight;
};
struct distribution {
    pid_t pids[50];
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
int counter =0;
void seedRandom();
void seedRandom() {
    std::time_t currentTime = std::time(nullptr);
    pid_t pid = getpid() + counter;
    unsigned int seed = static_cast<unsigned int>(currentTime) ^ static_cast<unsigned int>(pid);
    srand(seed);
    counter++;
}

// Get a random number in the range [min, max]
int getRandomRange(int min, int max) {
    return min + rand() % (max - min + 1);
}
int generateRandomNumber(int min, int max) {
    // Seed the random number generator with current time
    random_device rd;
    mt19937 gen(rd());
    
    // Define the distribution for random numbers within the specified range
    uniform_int_distribution<> dis(min, max);
    
    // Generate and return a random number within the specified range
    return dis(gen);
}


int main(int argc, char * argv []){
    resetSharedMemory();
    resetSemaphores();

    pid_t ppid = getppid();
    int collectorsTeams = readFromFile("collectorsCommitteesCount=");
    int splittingMembers = readFromFile("spilttersCommitteesCount=");
    int distributionTeams = readFromFile("distributersCommitteesCount=");
    int minimumStarvationRate = readFromFile("minimumStarvationRate=");
    int maximumStarvationRate = readFromFile("maximumStarvationRate=");
    int families= readFromFile("families=");
    int idfNo = readFromFile("idf=");
    key_t shm_keyStorage = ftok("/tmp", 'G');
    int shmid;
    char *shmptr;
    key_t shm_keyCollectors = ftok("/tmp", 'A'); // Using a constant value as the second argument for uniqueness
    key_t shm_keySplitters = ftok("/tmp", 'B'); // Using a constant value as the second argument for uniqueness
    key_t shm_keyDistributers = ftok("/tmp", 'd'); // Using a constant value as the second argument for uniqueness
    key_t shm_keyFamily = ftok("/tmp", 'f'); // Using a constant value as the second argument for uniqueness
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
    
    storage->totalWeight = 0;

    int shmid_collectors = shmget(shm_keyCollectors, sizeof(struct collectors), IPC_CREAT | 0666);
    if (shmid_collectors == -1) {
        perror("shmget (collectors)");
        exit(1);
    }

    int shmid_splitting = shmget(shm_keySplitters, sizeof(struct splitting), IPC_CREAT | 0666);
    if (shmid_splitting == -1) {
        perror("shmget (splitting)");
        exit(1);
    }

    int shmid_distribution = shmget(shm_keyDistributers, sizeof(struct distribution), IPC_CREAT | 0666);
    if (shmid_distribution == -1) {
        perror("shmget (distribution)");
        exit(1);
    }

    int shmid_families = shmget(shm_keyFamily, sizeof(struct families), IPC_CREAT | 0666);
    if (shmid_families  == -1) {
        perror("shmget (families)");
        return 1;
    }

    struct collectors * collectors_ptr = (struct collectors *)shmat(shmid_collectors, NULL, 0);
    if (collectors_ptr == (void *) -1) {
        perror("shmat (collectors)");
        exit(1);
    }

    struct splitting * splitting_ptr = (struct splitting *)shmat(shmid_splitting, NULL, 0);
    if (splitting_ptr == (void *) -1) {
        perror("shmat (splitting)");
        exit(1);
    }

    struct distribution * distribution_ptr = (struct distribution *)shmat(shmid_distribution, NULL, 0);
    if (distribution_ptr == (void *) -1) {
        perror("shmat (distribution)");
        exit(1);
    }
    struct families *families_ptr = (struct families *)shmat(shmid_families, NULL, 0);
    if (families_ptr== (void *)-1) {
        perror("shmat (families)");
        return 1;
    }

    for(int i=0; i<families;i++){    
        seedRandom();
        families_ptr->fam[i].starvationRate = generateRandomNumber(minimumStarvationRate, maximumStarvationRate);
        families_ptr->fam[i].status =0;
        families_ptr->fam[i].id =i;
    }
    for(int i=0; i<5; i++){

        if(!fork())
            execlp("./plane", "plane", to_string(i).c_str(), nullptr);
    }

    for(int i=0; i<idfNo; i++){

        if(!fork())
            execlp("./idf", "idf", to_string(i).c_str(), nullptr);
    }

    for(int i=0; i<collectorsTeams; i++){

        if(!fork())
            execlp("./collect", "collect", to_string(i).c_str(), nullptr);
    }

    for(int i=0; i<splittingMembers; i++){

        if(!fork())
            execlp("./split", "split", to_string(i).c_str(), nullptr);   
    }

    for(int i=0; i<distributionTeams; i++){

        if(!fork())
            execlp("./dist", "dist", to_string(i).c_str(), nullptr); 
    }

    for (int i = 0; i < 25; i++) {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << "Child process terminated abnormally" << std::endl;
        }
    }
    if (shmdt(collectors_ptr) == -1) {
        perror("shmdt (collectors)");
        exit(1);
    }

    if (shmdt(splitting_ptr) == -1) {
        perror("shmdt (splitting)");
        exit(1);
    }

    if (shmdt(distribution_ptr) == -1) {
        perror("shmdt (distribution)");
        exit(1);
    }
    if (shmdt(families_ptr) == -1) {
        perror("shmdt (distribution)");
        exit(1);
    }
    if (shmctl(shmid_collectors, IPC_RMID, NULL) == -1) {
        perror("shmctl (collectors)");
        return 1;
    }
    // Delete the shared memory segment
    if (shmctl(shmid_splitting, IPC_RMID, NULL) == -1) {
        perror("shmctl (splitting)");
        return 1;
    }
    // Delete the shared memory segment
    if (shmctl(shmid_distribution, IPC_RMID, NULL) == -1) {
        perror("shmctl (dist)");
        return 1;
    }
    // Delete the shared memory segment
    if (shmctl(shmid_families, IPC_RMID, NULL) == -1) {
        perror("shmctl (dist)");
        return 1;
    }
}


int readFromFile(std::string parameter) {
    std::string value = "";
    std::ifstream inputFile("vars.txt"); // Replace "vars.txt" with your file name
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return -1; // Return a default value indicating error
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        size_t equalPos = line.find(parameter);

        if (equalPos != std::string::npos) { // If parameter found
            // Extract the substring after the parameter
            value = line.substr(equalPos + parameter.length());

            // Print for debugging
            // std::cout << "Parameter: " << parameter << ", Extracted value: " << value << std::endl;

            // Convert the value string to an integer
            inputFile.close(); // Close the file before returning
            try {
                return std::stoi(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                return -1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << std::endl;
                return -1;
            }
        }
    }
    inputFile.close(); // Close the file

    std::cerr << "Parameter not found in the input file." << std::endl;
    return -1; // Return a default value indicating error
}



    void resetSemaphores(){
        system(" rm  /dev/shm/*");
    }
