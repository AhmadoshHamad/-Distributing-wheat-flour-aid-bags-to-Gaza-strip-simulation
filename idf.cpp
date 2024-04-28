#include "headers/all.h"


int main(){
    int shmid;
    char * shmPtr;
         // create a shared memory if exisits use it 
    if ( (shmid = shmget(KEY, MEMORY_SIZE, IPC_CREAT | 0666)) < 0 ) {
        perror("shmget fail");
        exit(1);
    }

        // return a pointer of the shared memory to use it 
    if ( (shmPtr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
        perror("shmat: parent");
        exit(2);
    }


   
    Container container(0,0);

    memcpy(&container,shmPtr,sizeof(Container));
    container.printDetails();




}