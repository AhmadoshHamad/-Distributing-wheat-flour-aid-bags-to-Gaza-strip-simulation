int readFromFile(std::string);

using namespace std;

#define SEM_KEY 12345 // Example key for the semaphore

struct sembuf lock = {0, -1, SEM_UNDO}; // Semaphore lock operation
struct sembuf unlock = {0, 1, SEM_UNDO}; // Semaphore unlock operation

void seedRandom();
int generateRandomNumber(int min, int max) {
    // Seed the random number generator with current time
    random_device rd;
    mt19937 gen(rd());
    
    // Define the distribution for random numbers within the specified range
    uniform_int_distribution<> dis(min, max);
    
    // Generate and return a random number within the specified range
    return dis(gen);
}
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
struct family{
    int starvationRate;
    int status;
    int id;
};
struct families{
    struct family fam[50];
};
int getRandomRange(int min, int max){
    seedRandom();
    return min + rand() % (max - min + 1);
}
void seedRandom() {
    std::time_t currentTime = std::time(nullptr);

    pid_t pid = getpid();

    unsigned int seed = static_cast<unsigned int>(currentTime) ^ static_cast<unsigned int>(pid);

    srand(seed);
}
bool checkFamilies(struct families *families_ptr, int n){
    int counter =0;
    for(int i=0; i< n; i++){
        if(families_ptr->fam[i].status==2){
            counter++;
        }
    }
    if(counter == n){
        return true;
    }
    else{
        return false;
    }
}
void selectionSort(struct families *families_ptr, int n) {
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (families_ptr->fam[j].starvationRate > families_ptr->fam[min_idx].starvationRate) {
                min_idx = j;
            }
        }
        int temp = families_ptr->fam[min_idx].starvationRate;
        families_ptr->fam[min_idx].starvationRate = families_ptr->fam[i].starvationRate;
        families_ptr->fam[i].starvationRate = temp;
        int status = families_ptr->fam[min_idx].status;
        families_ptr->fam[min_idx].status = families_ptr->fam[i].status;
        families_ptr->fam[i].status = status;
        int id = families_ptr->fam[min_idx].id;
        families_ptr->fam[min_idx].id = families_ptr->fam[i].id;
        families_ptr->fam[i].id = id;
    }
}
int findMaxStarvationIndex(struct families *families_ptr, int n) {
    int max_idx = -1;
    for (int i = 0; i < n; i++) {
        if (families_ptr->fam[i].status == 0 && (max_idx == -1 || families_ptr->fam[i].starvationRate > families_ptr->fam[max_idx].starvationRate)) {
            max_idx = i;
        }
    }
    return max_idx;
}
int energyDelay(int energy) {
    int  value;
    switch(energy/10){
    case 1:
        value = 9;
        break;
     case 2:
        value = 8;
        break;    
    case 3:
        value = 7;
        break;
    case 4:
        value = 6;
        break;
    case 5:
        value = 5;
        break;
    case 6:
        value = 4;
        break;
    case 7:
        value = 3;
        break;
    case 8:
        value = 2;
        break;
    case 9:
        value =1;
        break;
    default:
        value =1;
        break;        
    } 
  
    return value;
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
