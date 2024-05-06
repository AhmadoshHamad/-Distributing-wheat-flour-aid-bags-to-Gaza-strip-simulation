#include "headers/all.h"

int shmid; // shared memory id
char * shmPtr; // shared memory pointer


// to do : refill time of containers (infinite loop)

int main(int argc, char * argv []){
  int minimumRefillTime = readFromFile("minimumRefillTime=");
  int maximumRefillTime = readFromFile("maximumRefillTime=");
  int minimumContainers = readFromFile("minimumContainerCount=");
  int maximumContainers = readFromFile("maximumContainerCount=");
    Plane plane(getRandomRange(minimumRefillTime, maximumRefillTime),50,getRandomRange(minimumContainers, maximumContainers),3);
    plane.printDetails();
    resetSharedMemory();  

    for (int i=0; i< plane.containers; i++){
      
        char index[4]; // Convert integer index to string
        // plane.altidute+=i;
        snprintf(index, sizeof(index), "%zu", i);    
        if(!fork())
          execlp("./container","container",index);
        
        
        // usleep(500);


        sleep(plane.dropTime);
      }


      // for (size_t i = 0; i < 2; i++){
      //   wait(NULL);
      // }
      
   
    //   char * shmPtr = attachSharedMemory(create_OpenSharedMemory(10));
    //   shmPtr+=8;


    // for (size_t i = 0; i < 5; i++){
    //   Container container(0,0,0);
    //   memcpy(&container,shmPtr,sizeof(Container));
    //   container.printDetails();
    //   shmPtr += sizeof(Container);
    // }


      

}