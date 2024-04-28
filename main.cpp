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
using namespace std;

void printRazi(char a[]){
    printf("%s",a);
}


int main(int argc, char * argv []){

    printRazi("razi");



    // cout << argc << endl;
    cout << "number of arguments is " << argc << endl; 
        
    // argv order families then cargo planes then idf 
    for (size_t parameter = 1; parameter < argc; parameter++){// loop over the paremter passed with the ./main
       
       // convert each parameter to an integer and make childs for familes, then planes, then idf
      for (size_t i = 0; i < atoi(argv[parameter]); i++){
        char k[5];
        snprintf(k,sizeof(int),"%d",i);
        if(!fork())
            execlp("./family",k);
      }
      
        
    }



        // make the parent waits until all its chlidren die
    for (int i = 0; i < atoi(argv[1]); ++i) { 
        int status; // the status of the each child
         // the -1 means that the parent waits for any child to die and by performing it in a loop it will wait for all children 
        pid_t child_pid = waitpid(-1, &status, 0); 

        if (child_pid > 0) {
            cout << "Child process " << child_pid << " has terminated" << endl;
        }
    }
    



}