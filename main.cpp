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
#include <fstream>
using namespace std;
int planes=0;
int readFromFile(std::string);
int main(int argc, char * argv []){


    planes = readFromFile("planes=");
    // cout << argc << endl;
    cout << "number of arguments is " << argc << endl; 
    
    // argv order families then cargo planes then idf 
    for (size_t parameter = 1; parameter < argc; parameter++){// loop over the paremter passed with the ./main
       
       // convert each parameter to an integer and make childs for familes, then planes, then idf
      for (size_t i = 0; i < planes; i++){
        char k[5];
        snprintf(k,sizeof(int),"%d",i);
        if(!fork())
            execlp("./plane", k);
      }   
    }
    for (int i = 0; i < planes; ++i) { 
        int status; // the status of the each child
         // the -1 means that the parent waits for any child to die and by performing it in a loop it will wait for all children 
        pid_t child_pid = waitpid(-1, &status, 0); 

        if (child_pid > 0) {
            cout << "Child process " << child_pid << " has terminated" << endl;
        }
    }
}

int readFromFile(const char parameter[20]) {
    std::string value = "";
    std::ifstream inputFile("vars.txt"); // Replace "vars.txt" with your file name
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return -1; // Return a default value indicating error
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        size_t equalPos = line.find(parameter, 0, 20); // Searching for the parameter in the line

        if (equalPos != std::string::npos) { // If parameter found
            // Extract the substring after the parameter
            value = line.substr(equalPos + 1);

            // Convert the value string to an integer
            inputFile.close(); // Close the file before returning
            return std::stoi(value);
        }
    }
    inputFile.close(); // Close the file

    std::cerr << "Parameter not found in the input file." << std::endl;
    return -1; // Return a default value indicating error
}
int readFromFile(std::string parameter) {
    std::string value = "";
    std::ifstream inputFile("vars.txt"); // Replace "vars.txt" with your file name
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return -1; // Return a default value indicating error
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        size_t equalPos = line.find(parameter);

        if (equalPos != std::string::npos) { // If parameter found
            // Extract the substring after the parameter
            value = line.substr(equalPos + parameter.length());

            // Print for debugging
            std::cout << "Parameter: " << parameter << ", Extracted value: " << value << std::endl;

            // Convert the value string to an integer
            inputFile.close(); // Close the file before returning
            try {
                return std::stoi(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
                return -1;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << std::endl;
                return -1;
            }
        }
    }
    inputFile.close(); // Close the file

    std::cerr << "Parameter not found in the input file." << std::endl;
    return -1; // Return a default value indicating error
}
