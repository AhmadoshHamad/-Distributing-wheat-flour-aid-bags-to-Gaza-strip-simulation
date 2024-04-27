#ifndef SEMP_H
#define SEMP_H


sem_t * create_openSemaphore(char * sem_name){
    sem_t * mutex = sem_open(sem_name, O_CREAT, 0666, 1);
            if (mutex == SEM_FAILED) {
                perror("sem_open");
                exit(EXIT_FAILURE);
            }

    //cout << "created ! \n";
    return mutex;

}



#endif