#include "headers/all.h"

int shmid;
char * shmPtr;
sem_t *sem;


int main( int argc, char *argv []){
  shmid = create_OpenSharedMemory(10);
  shmPtr = attachSharedMemory(shmid);
  int start =0; // start of the shared pointer location
  memcpy(shmPtr,&start,sizeof(int));

  for (size_t i = 0; i < 5; i++){
      pid_t pid = fork();
       char index[2]; // Convert integer index to string
        snprintf(index, sizeof(index), "%zu", i);
       if(pid == 0 )
          execlp("./plane","plane",index);
  }
  
    for (size_t i = 0; i < 5; i++)
    {
      wait(NULL);
    }

    shmPtr+=4;


    for (size_t i = 0; i < 5; i++){
      Container container(0,0);
      memcpy(&container,shmPtr,sizeof(Container));
      container.printDetails();
      shmPtr += sizeof(Container);
    }
    
    



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