int readFromFile(std::string);
#define SEM_KEY 10 // Example key for the semaphore

struct sembuf lock = {0, -1, SEM_UNDO}; // Semaphore lock operation
struct sembuf unlock = {0, 1, SEM_UNDO}; // Semaphore unlock operation
using namespace std;
struct storageRoom{
    int totalWeight;
};

struct splitting{
    int pids[30];
};
struct distCommittee{
    int pidTeam;
    int energy;
    int status;
    int numberOfBags;
};
struct distribution {
    pid_t pids[50]; // Array to store process IDs
    struct distCommittee dist[50];
};
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
            // std::cout << "Parameter: " << parameter << ", Extracted value: " << value << std::endl;

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
