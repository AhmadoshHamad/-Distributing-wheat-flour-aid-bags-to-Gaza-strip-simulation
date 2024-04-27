#include "headers/all.h"

#define key 10

const char* SEM_MUTEX_NAME = "/mutex";
int shmid; // shared memory id
char * shmPtr; // shared memory pointer

void* consumer(void* arg) {
    sem_t * mutex;
    
         mutex = sem_open(SEM_MUTEX_NAME, O_CREAT , 0666, 1);
            if (mutex == SEM_FAILED) {
                perror("sem_open");
                exit(EXIT_FAILURE);
            }
        
        sem_wait(mutex);


                    // create a shared memory if exisits use it 
            if ( (shmid = shmget(key, MEMORY_SIZE, IPC_CREAT | 0666)) < 0 ) {
                perror("shmget fail");
                exit(1);
            }

                // return a pointer of the shared memory to use it 
            if ( (shmPtr = (char *) shmat(shmid, 0, 0)) == (char *) -1 ) {
                perror("shmat: parent");
                exit(2);
            }

            int readed;

            memcpy(&readed,shmPtr,sizeof(int));
            cout << "consumed value is " << readed << endl;
            readed++;
            memcpy(shmPtr,&readed,sizeof(int));

        
        std::cout << "Consumed: " << std::endl;
        //sleep(1);

        sem_post(mutex);
       
    
    return NULL;
}

int main(){

consumer(NULL);
    
}