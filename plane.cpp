#include "headers/all.h"

int shmid; // shared memory id
char * shmPtr; // shared memory pointer


int main(int argc, char * argv []){
    Plane plane(5,50,1,3);
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


      for (size_t i = 0; i < 2; i++){
        wait(NULL);
      }
      
   
    //   char * shmPtr = attachSharedMemory(create_OpenSharedMemory(10));
    //   shmPtr+=8;


    // for (size_t i = 0; i < 5; i++){
    //   Container container(0,0,0);
    //   memcpy(&container,shmPtr,sizeof(Container));
    //   container.printDetails();
    //   shmPtr += sizeof(Container);
    // }


      

}