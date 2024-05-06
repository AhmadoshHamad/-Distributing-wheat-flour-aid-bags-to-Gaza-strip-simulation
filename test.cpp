#include "headers/all.h"
#include <semaphore.h>
int shmid;
char * shmPtr;
// sem_t *sem;
char idf_semaphore[20] = "/idfsem";
// char * container_sem = "/containerSemapore";

void close_sem(char *);

int main( int argc, char *argv []){
  reset();
  // just empty the shared memory
  // close_sem(idf_semaphore);
  sem_open(idf_semaphore,O_CREAT,0660,1);
  // close_sem(container_sem);
 
  // sleep(5);
  char zeros [4000];
  memset(zeros,0,4000);

  shmid = create_OpenSharedMemory(10); 
  shmPtr = attachSharedMemory(shmid);
  memcpy(shmPtr,&zeros,sizeof(zeros));
  int start =0; // start of the shared pointer location
  memcpy(shmPtr,&start,sizeof(int));
  pid_t pid;

for (size_t i = 0; i < 5; i++){
       pid = fork();
       char index[2]; // Convert integer index to string
        snprintf(index, sizeof(index), "%zu", i);
       if(pid == 0 ){
          execlp("./idf","idf",index);
       }
  }



  for (size_t i = 0; i < 5; i++){
       pid = fork();
       char index[2]; // Convert integer index to string
        snprintf(index, sizeof(index), "%zu", i);
       if(pid == 0 )
          execlp("./plane","plane",index);
  }

 

  
    for (size_t i = 0; i < 10; i++)
    {
      wait(NULL);
    }
    // kill all idf ffor testing purposes only
    // system("killall idf");

    shmPtr+=8;


}



 void close_sem(char* semName){

     if (sem_unlink(semName) == -1) {
        perror("sem_unlink");
        // exit(EXIT_FAILURE);
    }

  }

  
    // for (size_t i = 0; i < 5; i++){
    //   Container container(0,0,0);
    //   memcpy(&container,shmPtr,sizeof(Container));
    //   container.printDetails();
    //   shmPtr += sizeof(Container);
    // }
    
    
   // Close and unlink the semaphore
    // if (sem_close(sem) == -1) {
    //     perror("sem_close");
    //     exit(EXIT_FAILURE);
    // }
    // if (sem_unlink(sem_name) == -1) {
    //     perror("sem_unlink");
    //     exit(EXIT_FAILURE);
    // }

