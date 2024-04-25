#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
// #include <sys/wait.h>
#include <signal.h>
#include "plane.h"
#include <cstdlib> // for srand and rand
#include <ctime>   // for time
using namespace std;

int readFromFile(std::string);
int getRandomRange(int, int);
void seedRandom();
int main(int argc, char * argv []){
    seedRandom();
    int minimumContainerCount = readFromFile("minimumContainerCount=");
    int maximumContainerCount = readFromFile("maximumContainerCount=");
    int minimumDropContainerTime = readFromFile("minimumDropContainerTime=");
    int maximumDropContainerTime = readFromFile("maximumDropContainerTime=");
    int minimumRefillTime = readFromFile("minimumRefillTime=");
    int maximumRefillTime = readFromFile("maximumRefillTime=");

    Plane plane(
    getRandomRange(minimumRefillTime,maximumRefillTime),
    5,
    getRandomRange(minimumContainerCount, maximumContainerCount),
    getRandomRange(minimumDropContainerTime, maximumDropContainerTime));
    plane.printDetails();
    return 0;
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
int getRandomRange(int min, int max){
    return min + rand() % (max - min + 1);
}
void seedRandom() {
    // Get current time
    std::time_t currentTime = std::time(nullptr);

    // Get process ID
    pid_t pid = getpid();

    // Combine time and process ID to generate a unique seed
    unsigned int seed = static_cast<unsigned int>(currentTime) ^ static_cast<unsigned int>(pid);

    // Seed the random number generator
    srand(seed);
}
