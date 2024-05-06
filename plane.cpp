#include "headers/all.h"

int shmid; // shared memory id
char * shmPtr; // shared memory pointer


// to do : refill time of containers (infinite loop)

int main(int argc, char * argv []){
  int minimumRefillTime = readFromFile("minimumRefillTime=");
  int maximumRefillTime = readFromFile("maximumRefillTime=");
  int minimumContainers = readFromFile("minimumContainerCount=");
  int maximumContainers = readFromFile("maximumContainerCount=");
  int minPlaneHeight = readFromFile("minPlaneHeight=");
  int maxPlaneHeight = readFromFile("maxPlaneHeight=");
  int minimumDropContainerTime = readFromFile("minimumDropContainerTime=");
  int maximumDropContainerTime = readFromFile("maximumDropContainerTime=");
    // initialise plane with random altidude, noContainers and refill time
    Plane plane(getRandomRange(minimumRefillTime, maximumRefillTime),    getRandomRange(minPlaneHeight,maxPlaneHeight),     getRandomRange(minimumContainers, maximumContainers),getRandomRange(minimumDropContainerTime,maximumDropContainerTime));
    plane.printDetails();
    resetSharedMemory();  

    while (1){
      
    for (int i=0; i< plane.containers; i++){
      
        char index[4]; // Convert integer index to string
        // plane.altidute+=i;
        snprintf(index, sizeof(index), "%zu", plane.altidute);    
        if(!fork())
          execlp("./container","container",index);

        sleep(plane.dropTime); // time between dropping containers

      }

      sleep(plane.refillTime); // refill time of plane
    }

}