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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

// incldue plane class
#include "headers/plane.h"
#include "headers/SharedMemory.h"


#define MEMORY_SIZE  4096
#define key 10



using namespace std;
int shmid;
char * shmPtr;
sem_t *sem;


int main( int argc, char *argv []){
  string str = std::to_string(getpid());
  const char* sem_name = str.c_str();
  

sem = sem_open(sem_name,O_CREAT,0666,1);

        if (sem == SEM_FAILED) {
            // Semaphore already exists, open it
            sem = sem_open(sem_name, 0);
            if (sem == SEM_FAILED) {
                perror("sem_open");
                exit(EXIT_FAILURE);
            }
        }
        else{
          cout << "Im heeeeeeere";
        }


        sleep(2);



   // Close and unlink the semaphore
    // if (sem_close(sem) == -1) {
    //     perror("sem_close");
    //     exit(EXIT_FAILURE);
    // }
    // if (sem_unlink(sem_name) == -1) {
    //     perror("sem_unlink");
    //     exit(EXIT_FAILURE);
    // }















    // if ( (shmid = shmget(IPC_PRIVATE, MEMORY_SIZE, IPC_CREAT | 0644)) < 0 ) {
    //     perror("shmget fail");
    //     exit(1);
    // }

    //          // return a pointer of the shared memory to use it 
    // if ( (shmPtr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
    //     perror("shmat: parent");
    //     exit(2);
    // }




}




// extern etext, edata, end;

// main(void){
//   printf("")

//   pid_t    pid;
//   int      shmid;
//   char     c, *shm, *s;
  

  

  
//   printf("Addresses in parent\n\n");
//   printf("shared mem: %X etext: %X edata: %X end: %X\n\n",
// 	 shm, &etext, &edata, &end);
  
//   s = shm;
  
//   for ( c = 'A'; c <= 'Z'; ++c ) /* put some info there */
//     *s++ = c;
  
//   *s = NULL;
  
//   printf("In parent before fork, memory is: %s\n", shm);
  
//   pid = fork();
  
//   switch ( pid ) {
//   case -1:
//     perror("fork ");
//     exit(3);
    
//   default:
//     sleep(5);
//     printf("\nIn parent after fork, memory is: %s\n", shm);
//     printf("Parent removing shared memory\n");
//     shmdt(shm);
//     shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
//     exit(0);

//   case 0:
//     printf("In child after fork, memory is:   %s\n", shm);
//     for (; *shm; ++shm )
//       *shm += 32;
    
//     shmdt(shm);
//     exit(0);
//   }
// }
