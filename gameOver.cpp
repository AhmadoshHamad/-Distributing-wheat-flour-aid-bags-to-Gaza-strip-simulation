#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No PID provided." << std::endl;
        return 1; 
    }

    
    int childID = atoi(argv[1]);

   
    if (kill(-childID, SIGKILL) == 0) {
        std::cout << "Process with PID " << childID << " has been terminated." << std::endl;
    } else {
        std::cerr << "Error: Failed to terminate process with PID " << childID << std::endl;
    }

    return 0; 
}
