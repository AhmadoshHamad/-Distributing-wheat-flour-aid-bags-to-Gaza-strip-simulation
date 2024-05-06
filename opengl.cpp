#include <GL/glut.h>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm> 
#include <fstream> 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <random>
#include <signal.h>
#include <sys/wait.h>

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

using namespace std;
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
struct Square {
    float x;
    int number;
    float height;
    bool moveRight;
    int refillInterval;
    int dropInterval;
    int originalNumber;
    int lastDropTime;
    int lastRefillTime;
};
struct DropPoint {
    float x;
    float y;
    float height;
    int health;
    bool active;
};
struct IDF{
    int shots;
    int shotInterval;
    int damageRate; 
};
struct Collecters{
    float x;
    float y;
    int containers;
    int health;
    int timer;
};
struct Spliters{
    float x;
    float y;
    int weight;
    int timer;
};
struct Families{
    int starvation;
};
struct Distributers{
    float x;
    float y;
    int bags;
    int health;
    bool haveBags;
    float orginalX;
};
struct colleCommittee{
    int pidTeam[5];
    int energy[5];
    int bag ; // the wheat flour containers

};
struct collectors {
    pid_t pids[30]; // Array to store process IDs
    struct colleCommittee colle[20];
};

// for squares
float squareWidth = 0.1f;
float squareHeight = 0.1f;
float spacing = 0.1f;

// variables
int numSquares;
int numidf;
int numShots;
int shotInterval;
int damageRate;
int duration;
int FamiliesNum;
int shotsCount[20]={0};

// given
int numcoll;
int spliternum;
int distributersNum;
int timePerStarvationRate;
int familiesStarvationRate;
int wheatBagsCount;
int energyDepletionRate;
int distributersMinimumCount;
int shotContainersCount;
int collectorsDeathCount;
int distributersDeathCount;
int crashedPlanesCount;
int familyDeathRate;
int familyThreshold;
int  minimumFamilies;
int ContainerWeight;


// timers
int timer2=0;
int childID;
int timer3=10;
int deathrate=30;
int timer4=0;
int timer5=10;
int counterDrop=0;
int counter=0;
int counterCollect=0;
int counterDist=0;
int timerIDF=0;
int counterPlane=0;
int counterfamily=0;
int counterFamilies=0;


Square *squares;
DropPoint *dropPoints;
IDF *idf;
Collecters *collecters;
Spliters *spliters;
Families *families;
Distributers*distributers;
int maxDropPoints = 100;
bool animate = true; // Flag to control animation
bool gameOver = false; // Flag to indicate if the game is over

void drawSquare(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
void drawText(float x, float y, string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}
void handleShots(int value) {
    for (int i = 0; i < numidf; i++)
    {
        shotsCount[i]++;
    }
    
    
    for (int i = 0; i < numidf; ++i) {
        if (shotsCount[i] == idf[i].shotInterval) {
            shotsCount[i] = 0;

            int highestDropIndex = 0;
            for (int z = 0; z < maxDropPoints; ++z) {
                if (dropPoints[z].y > dropPoints[highestDropIndex].y && dropPoints[z].health == 100) {
                    highestDropIndex = z;
                }
            }
            for (int j = 0; j < idf[i].shots; ++j) {
                if ( dropPoints[highestDropIndex].active) {
                        if (dropPoints[highestDropIndex].health!=0){
                        dropPoints[highestDropIndex].health -= idf[i].damageRate;
                        counterDrop++;
                        if (dropPoints[highestDropIndex].health <= 0) {
                            dropPoints[highestDropIndex].health=0;
                        }
                    }
                }
            }
        }
    }
}
void decreaseNumbers(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000;
    static bool firstDrop = false; 
    for (int i = 0; i < numSquares; ++i) {
        if (currentTime - squares[i].lastDropTime >= squares[i].dropInterval && squares[i].number > 0) {
            squares[i].number--;
            squares[i].lastDropTime = currentTime;
            for (int j = 0; j < maxDropPoints; ++j) {
                if (!dropPoints[j].active) {
                    dropPoints[j].x = squares[i].x + squareWidth / 2;
                    dropPoints[j].y = squares[i].height; 
                    dropPoints[j].height = 0.7f;
                    dropPoints[j].health = ContainerWeight; 
                    dropPoints[j].active = true; 
                    break;
                }
            }
            firstDrop = true;
        }
        if (squares[i].number == 0 && squares[i].lastRefillTime == 0) {
            squares[i].lastRefillTime = currentTime;
        }
        if (squares[i].number == 0 && currentTime - squares[i].lastRefillTime >= squares[i].refillInterval) {
            squares[i].number = squares[i].originalNumber;
            squares[i].lastRefillTime = 0;
        }
    }
  for (int i = 0; i < maxDropPoints; ++i) {
    if (dropPoints[i].active) {
        float decrement = 0.1f * (1.0f - dropPoints[i].y); 
        dropPoints[i].y -= decrement;
        if (dropPoints[i].y < -0.9f && dropPoints[i].health == 0) {
            dropPoints[i].active = false;
        } else if (dropPoints[i].y < -0.9f) {
            dropPoints[i].y = -0.9f;
        }
    }
}
    if (firstDrop) {
        glutTimerFunc(1000, handleShots, 0);
    }
    glutPostRedisplay();
    glutTimerFunc(1000, decreaseNumbers, 0);
}
void initializeSquares() {
    float startX = -1.0f;
    for (int i = 0; i < numSquares; ++i) {
        squares[i].x = startX + i * (squareWidth + spacing);
        squares[i].number = rand() % 10 + 1;
        squares[i].originalNumber = squares[i].number;
        squares[i].moveRight = true;
        squares[i].dropInterval = 5;
        squares[i].height = 0.4f + ((float)rand() / RAND_MAX) * 0.4f;
        squares[i].lastDropTime = 0;
        squares[i].refillInterval = 10;
        squares[i].lastRefillTime = 0;
    }
    dropPoints = new DropPoint[maxDropPoints];
    for (int i = 0; i < maxDropPoints; ++i) {
        dropPoints[i].active = false;
        dropPoints[i].health = ContainerWeight; 
    }
    for (int i = 0; i < numidf; ++i) {
       idf[i].shots=numShots+i;
       idf[i].shotInterval=shotInterval;
       idf[i].damageRate=damageRate;
    }
    float spacing = 0.05f;
    key_t shm_keyCollectors = ftok("/tmp", 'A'); 
    int shmid_collectors = shmget(shm_keyCollectors, sizeof(struct collectors), IPC_CREAT | 0666);
    if (shmid_collectors == -1) {
        perror("shmget (collectors)");
    }
    struct collectors *collectors_ptr = (struct collectors *)shmat(shmid_collectors, NULL, 0);
    if (collectors_ptr == (void *)-1) {
        perror("shmat (collectors)");
    }
     for (int i = 0; i < numcoll; ++i) {
       collecters[i].health=collectors_ptr->colle[i].energy[0];
       collecters[i].containers=0;
       collecters[i].y =-0.9f;
       collecters[i].x = (i * (squareWidth - 0.05f + spacing))-0.2f;
       collecters[i].timer=0;
    }
    for (int i = 0; i < spliternum; ++i) {
       spliters[i].weight=0;
       spliters[i].y =-0.7f;
       spliters[i].x = (i * (squareWidth - 0.05f + spacing))-0.2f;
       spliters[i].timer=0;
    }
        key_t shm_keyFamily = ftok("/tmp", 'f'); // Using a constant value as the second argument for uniqueness
        int shmid_families = shmget(shm_keyFamily, sizeof(struct families), IPC_CREAT | 0666);
        if (shmid_families  == -1) {
            perror("shmget (families)");
            
        }
        struct families *families_ptr = (struct families *)shmat(shmid_families, NULL, 0);
        if (families_ptr == (void *)-1) {
            perror("shmat (families)");
        }
    for (int i = 0; i< FamiliesNum; i++)
    {
        families[i].starvation =families_ptr->fam[i].starvationRate ; 
    }
    key_t shm_key = ftok("/tmp", 'd');
    int shmid_distribution = shmget(shm_key, sizeof(struct distribution), IPC_CREAT | 0666);
    if (shmid_distribution == -1) {
        perror("shmget (dist)");
    }
    struct distribution *distributers_ptr = (struct distribution *)shmat(shmid_distribution, NULL, 0);
    if (distributers_ptr == (void *)-1) {
        perror("shmat (dist)");
    }
    for(int i=0; i<distributersNum;i++){
        distributers[i].bags=wheatBagsCount;
        distributers[i].health=distributers_ptr->dist[i].energy;
        distributers[i].y =-1.0f;
        distributers[i].x = -i * (squareWidth - 0.05f + spacing );
        distributers[i].orginalX = (-i * (squareWidth - 0.05f + spacing ))-0.1f;
        distributers[i].haveBags=false;
    }
}
void gameOverFunc(int value) {
    animate = false;
    gameOver = true;
    glutPostRedisplay();
    
}
void display() {
    
    if (animate ){

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0.0f, 0.7f, 1.0f); 
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.9f);     
    glVertex2f(1.0f, -0.9f);    
    glVertex2f(1.0f, 1.0f); 
    glVertex2f(-1.0f, 1.0f);    
    glEnd();
    glColor3f(0.5f, 0.0f, 0.5f); 
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glVertex2f(-1.0f, -0.3f);
    glVertex2f(-0.8f, -0.3f);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f); 
    glRasterPos2f(-1.0f, -0.45f); 
    const char* text = "Families";
    for (const char* c = text; *c; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glColor3f(1.0f, 1.0f, 1.0f); 
    glRasterPos2f(-1.0f, 0.0f);
    text = "IDF";
    for (const char* c = text; *c; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); 
    }
    glColor3f(1.0f, 1.0f, 1.0f); 
    glRasterPos2f(0.0f, 0.9f);
    text = "Planes";
    for (const char* c = text; *c; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); 
    }
    glColor3f(1.0f, 1.0f, 1.0f); 
    glRasterPos2f(0.5f, -0.5f);
    text = "Collecters";
    for (const char* c = text; *c; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); 
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.2f, -0.5f);
    text = "Splitters (Storage)";
    for (const char* c = text; *c; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.7f, -0.7f);
    text = "Distributers";
    for (const char* c = text; *c; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glColor3f(0.0f, 0.8f, 0.0f); 
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);      
    glVertex2f(1.0f, -1.0f);    
    glVertex2f(1.0f, -0.9f); 
    glVertex2f(-1.0f, -0.9f);    
    glEnd();
    float startY = -0.7f; 
    float squareGap = 0.3f; 
    float y = -0.3f; 
    float yf = -0.9f; 
    float yColl=-0.9;
    float verticalSpacing = 0.005f;
    for (int i = 0; i < 1; ++i) {
        float x =-0.2f+  i * (squareWidth + squareGap); 
        drawSquare(x, -1.0, squareWidth+0.3f, squareHeight+0.35f, 0.0f, 0.0f, 1.0f);
    }
    for (int i = 0; i < numcoll; ++i) {
        drawSquare(collecters[i].x, collecters[i].y, squareWidth - 0.05f, squareHeight - 0.05f, 0.5f, 0.0f, 0.5f);
        string numText = to_string(collecters[i].health);
        if(collecters[i].health>0){
            drawText(collecters[i].x + squareWidth / 2 - 0.03f, yColl + squareHeight + 0.03f, numText);
        }       
        for(int j=0; j<maxDropPoints;j++){
            if(dropPoints[j].y == -0.9f && dropPoints[j].active && dropPoints[j].health>0){
                for(int k=0; k<numcoll;k++){
                    if(collecters[k].containers==0){
                        collecters[k].x = dropPoints[j].x;
                        collecters[k].containers+=dropPoints[j].health;
                        collecters[k].health-=energyDepletionRate;
                        dropPoints[j].active=false;
                        break;         
                    }else{
                        if(collecters[k].timer!=3){
                           collecters[k].timer++;
                        }
                    }
                }
            }     
    }
        
        numText = to_string(collecters[i].health);
        if(collecters[i].health>0){
            drawText(collecters[i].x + squareWidth / 2 - 0.03f, yColl + squareHeight + 0.03f, numText);
        }  
      
    }

    for (int i = 0; i < distributersNum; ++i) {        
        drawSquare(distributers[i].x +0.1f, distributers[i].y, squareWidth - 0.05f, squareHeight - 0.05f, 1.0f, 0.7f, 0.7f);
        string numText = to_string(distributers[i].health);
        if(distributers[i].health>0){
            drawText(distributers[i].x+0.1f + squareWidth / 2 - 0.03f, distributers[i].y + squareHeight - 0.03f, numText);
        }        
    }
    for (int j = 0; j < spliternum; j++){
        for (int i = 0; i < distributersNum; i++)
        {
            if(spliters[j].weight>distributers[i].bags &&spliters[j].timer==1 &&  distributers[i].haveBags==false ){ 
                spliters[j].weight-=(distributers[i].bags*10);
                spliters[j].timer=0;
                distributers[i].haveBags=true;
            }
            int mostStarvationIndex = 0;
            for (int i = 1; i < FamiliesNum; ++i) {
                if (families[i].starvation > families[mostStarvationIndex].starvation) {
                    mostStarvationIndex = i;
                }
            }
            if(distributers[i].haveBags==true && distributers[i].x>=-0.89f){
                distributers[i].x-=0.05f;
            }  
            if(distributers[i].x<=-0.89f )
            {   if(families[mostStarvationIndex].starvation-(distributers[i].bags*10)<=0){
                   families[mostStarvationIndex].starvation=0;
                }
                else{

                    families[mostStarvationIndex].starvation-=distributers[i].bags*10;
                }
                distributers[i].health-=energyDepletionRate;
                distributers[i].x=distributers[i].orginalX;
                distributers[i].haveBags=true;

            }
        }
    }
    for (int i = 0; i < spliternum; i++)
    {

        string numText = to_string(spliters[i].weight);
        drawText(spliters[i].x + squareWidth / 2 - 0.03f, yColl + squareHeight + 0.17f, numText);
        drawSquare(spliters[i].x, spliters[i].y, squareWidth - 0.05f, squareHeight - 0.05f, 0.5f, 0.5f, 0.5f);
                            for(int k=0; k<numcoll;k++){
                                if(collecters[k].containers>0 && collecters[k].timer==3){
                                    collecters[k].x =(k * (squareWidth - 0.05f + spacing))-0.3f;
                                    spliters[i].weight+=collecters[k].containers;
                                    drawSquare(spliters[i].x, spliters[i].y, squareWidth - 0.05f, squareHeight - 0.05f, 1.0f, 0.0f, 0.0f);
                                    collecters[k].containers=0;
                                    collecters[k].timer=0;  
                                    if(spliters[i].timer!=1){
                                       spliters[i].timer++;
                                    }
                                    break;          
                                } 
                            } 
        numText = to_string(spliters[i].weight);
        drawText(spliters[i].x + squareWidth / 2 - 0.03f, yColl + squareHeight + 0.17f, numText);                    
    }
    timer2++;
    for (int i = 0; i < FamiliesNum;) {
        float x = -0.89f ; 
        drawSquare(x , yf, squareWidth -0.05f, squareHeight-0.05f, 0.7f, 1.0f, 0.7f);
        string numText = to_string(families[i].starvation);
        drawText(x + (squareWidth / 2)-0.05f , yf + squareHeight-0.05f , numText);
        if (timer2==timePerStarvationRate){
        if(families[i].starvation+familiesStarvationRate>=100){
             families[i].starvation=100;
        }else{
          families[i].starvation+=familiesStarvationRate;
            if (families[i].starvation >= familyThreshold){
            counterFamilies++;
            }
        }
        if (families[i].starvation>=100){
            for (int j = i; j < FamiliesNum - 1; ++j) {
                families[j] = families[j + 1];
            }
            FamiliesNum--; 
            counterfamily++;
        }
        if(families[i+1].starvation+familiesStarvationRate>=100){
            families[i+1].starvation=100;
        }else{
          families[i+1].starvation+=familiesStarvationRate;
            if (families[i+1].starvation >= familyThreshold){
            counterFamilies++;
            }
        }
        if (families[i+1].starvation>=100){
            for (int j = i; j < FamiliesNum - 1; ++j) {
                families[j] = families[j + 1];
            }
            FamiliesNum--; 
            counterfamily++;
            continue;
        } 
        }
        if(i+1 == FamiliesNum)
             break;
        numText = to_string(families[i+1].starvation);
        drawText(x + (squareWidth / 2)-0.13f , yf + squareHeight-0.05f , numText); 
        drawSquare(x-0.08f, yf, squareWidth -0.05f, squareHeight-0.05f, 0.7f, 1.0f, 0.7f);
        yf += verticalSpacing + squareHeight; 
        i+=2;

    }
    if (timer2==5){
      timer2=0;
    } 
    for (int i = 0; i < numidf;) {
        float x = -0.89f ;
        if(shotsCount[i]==idf[i].shotInterval-1){
            drawSquare(x, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 0.0f, 0.0f);
        }else{
            drawSquare(x, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 1.0f, 0.0f);
        }
        
        
        
        timer3--;
        if(timer3<=0){
            timer3=10;
           int leastCollectorIndex = 0; // Assume the first collector has the lowest health initially
            for (int i = 1; i < numcoll; i++) {
                if (collecters[i].health < collecters[leastCollectorIndex].health) {
                    leastCollectorIndex = i; // Update the index if a collector with lower health is found
                }
            }
            if (collecters[leastCollectorIndex].health<deathrate){
            // Shift elements to remove the collector with the lowest health
            for (int i = leastCollectorIndex; i < numcoll - 1; i++) {
                collecters[i] = collecters[i + 1]; // Shift elements to the left
            }
            drawSquare(x, y, squareWidth -0.05f, squareHeight-0.05f, 0.0f, 0.0f, 1.0f);

            // Decrement the number of collectors since one collector is removed
            numcoll--;
            counterCollect++;
            timer4++;
                   
            }

            
        }
       
        if (timer4==1){
            timer4=0;
            if (spliternum > 0) {
                numcoll++; // Increment the number of collectors
                // Shift the elements in the splitters array to remove the first splitter
                for (int k = 0; k < spliternum - 1; k++) {
                    spliters[k] = spliters[k + 1]; // Shift elements to the left
                }
                
                // Decrement the number of splitters since one splitter is removed
                spliternum--;
            }
          
        }

        
        
        
        if(i+1 == numidf)
            break;
        if(shotsCount[i+1]==idf[i+1].shotInterval-1){
              drawSquare(x-0.08f, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 0.0f, 0.0f);
        }else{
            drawSquare(x-0.08f, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 1.0f, 0.0f);
        }
        timer5--;
        if(timer5<=0){
            timer5=10;
           int leastDIndex = 0; // Assume the first collector has the lowest health initially
            for (int i = 1; i < distributersNum; i++) {
                if (distributers[i].health < distributers[leastDIndex].health) {
                    leastDIndex = i; // Update the index if a collector with lower health is found
                }
            }
            if (distributers[leastDIndex].health<deathrate){
            // Shift elements to remove the collector with the lowest health
            for (int i = leastDIndex; i < distributersNum - 1; i++) {
                distributers[i] = distributers[i + 1]; // Shift elements to the left
            }
            drawSquare(x-0.08f, y, squareWidth -0.05f, squareHeight-0.05f, 0.0f, 0.0f, 1.0f);

            // Decrement the number of collectors since one collector is removed
            counterDist++;
            distributersNum--;
            counter++;
            
                   
            }

            
        }
       
            if (spliternum > 0 && counter==distributersMinimumCount ) {
                counter=0;
                distributersMinimumCount=1;
                distributersNum++; // Increment the number of collectors
                distributers[distributersNum-1].health=100;
                // Shift the elements in the splitters array to remove the first splitter
                for (int k = 0; k < spliternum - 1; k++) {
                    spliters[k] = spliters[k + 1]; // Shift elements to the left
                }
                
                // Decrement the number of splitters since one splitter is removed
                spliternum--;
            }
        

        y += verticalSpacing + squareHeight; // Update y-coordinate for the next square
        i+=2;
    }
    for (int i = 0; i < numSquares; ++i) {
        float x = squares[i].x;
        float y = squares[i].height;

        if (squares[i].number == 0) {
            // drawSquare(x, y, squareWidth, squareHeight, 0.0f, 1.0f, 0.0f);
        } else {
            drawSquare(x, y, squareWidth, squareHeight, 1.0f, 0.0f, 0.0f);
        }

        string numText = to_string(squares[i].number);
        if(squares[i].number>0){
            drawText(x + squareWidth / 2 - 0.03f, y + squareHeight + 0.03f, numText);
        }

        if (squares[i].moveRight) {
            squares[i].x += 0.1f;
            if (squares[i].x > 1.0f)
                squares[i].moveRight = false;
        } else {
            squares[i].x -= 0.1f;
            if (squares[i].x < 0.2f )
                squares[i].moveRight = true;
        }
      for (int j = 0; j < numSquares; ++j) {
        if (squares[i].x == squares[j].x && squares[i].height == squares[j].height && i!=j) {
            for (int k = j; k < numSquares - 1; ++k) {
                squares[k] = squares[k + 1];
            }
            counterPlane++;
            --numSquares;
            --j;
        }
        }
    }
    for (int i = 0; i < maxDropPoints; ++i) {
        if (dropPoints[i].active) {
         if (dropPoints[i].health == 0) {
            glColor3f(1.0f, 0.0f, 0.0f);
        } else {
            glColor3f(1.0f, 1.0f, 1.0f); 
        }
        glBegin(GL_QUADS); 
        float size = 0.05f; 
        float halfSize = size / 2.0f;
        glVertex2f(dropPoints[i].x - halfSize, dropPoints[i].y + halfSize);
        glVertex2f(dropPoints[i].x + halfSize, dropPoints[i].y + halfSize); 
        glVertex2f(dropPoints[i].x + halfSize, dropPoints[i].y - halfSize); 
        glVertex2f(dropPoints[i].x - halfSize, dropPoints[i].y - halfSize); 
        glEnd();
        float textX = dropPoints[i].x - 0.03f; 
        float textY = dropPoints[i].y + size ; 
        string numText = to_string(dropPoints[i].health);
        drawText(textX, textY, numText);
    }

}
     
     
     
    if(spliternum <=1 || counterDrop>=shotContainersCount || counterCollect >= collectorsDeathCount || counterDist >= distributersDeathCount || counterPlane>=crashedPlanesCount || counterfamily>=familyDeathRate || counterFamilies>=minimumFamilies ){
       
        
        animate =false;
        gameOver =true;
     }
     glutSwapBuffers();
    }else if( gameOver ){
        glColor3f(1.0f, 1.0f, 1.0f); 
        glRasterPos2f(-0.2f, 0.0f);
        const char* text = "Game over";
        for (const char* c = text; *c; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); 
        }
        long long  r = 2000000000;
        glutSwapBuffers();
        for (int i = 0; i <r ; i++); 
        pid_t pid = fork();  
        if (pid == -1) {
            perror("fork");
        } else if (pid == 0) {
            cout << "r";
            char index[4]; // Convert integer index to string
            snprintf(index, sizeof(index), "%zu", childID);
            execlp("./gameOver", "gameOver",index);
            perror("execlp");
        }
    }
  

}
int main(int argc, char** argv) {

    numcoll=readFromFile("collectorsCommitteesCount=");
    spliternum=readFromFile("spilttersCommitteesCount=");
    distributersNum=readFromFile("distributersCommitteesCount=");
    numSquares =  readFromFile("planes=");
    numidf =readFromFile("idf=");
    FamiliesNum = readFromFile("families=");
    duration =readFromFile("runTime=");
    numShots = readFromFile("numberOfShots=");
    shotInterval = readFromFile("shotsInterval=");
    damageRate = readFromFile("damageRate=");
    timePerStarvationRate =readFromFile("timePerStarvationRate=");
    familiesStarvationRate=readFromFile("familiesStarvationRate=");
    wheatBagsCount=readFromFile("wheatBagsCount=");
    energyDepletionRate=readFromFile("energyDepletionRate=");
    distributersMinimumCount=readFromFile("distributersMinimumCount=");
    shotContainersCount = readFromFile("shotContainersCount=");
    collectorsDeathCount = readFromFile("collectorsDeathCount=");
    distributersDeathCount =readFromFile("distributersDeathCount=");
    crashedPlanesCount=readFromFile("crashedPlanesCount=");
    familyDeathRate =readFromFile("familyDeathRate=");
    familyThreshold=readFromFile("familyThreshold=");
    minimumFamilies =readFromFile("minimumFamilies=");
    ContainerWeight =readFromFile("ContainerWeight=");
    pid_t pid = fork();  
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        childID = getpid();
        // In child process, execute main program
        execlp("./main", "main", NULL);

        // If execlp fails, print error
        perror("execlp");
        return EXIT_FAILURE;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Project2");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    

    squares = new Square[numSquares];
    idf =new IDF[numidf];
    collecters= new Collecters[numcoll];
    spliters = new Spliters[spliternum];
    families =new Families[FamiliesNum];
    distributers =new Distributers[distributersNum];

    srand(time(nullptr));
    initializeSquares();
    glutDisplayFunc(display);
    
   

    glutTimerFunc(duration * 1000, gameOverFunc, 0);
    glutTimerFunc(1000, decreaseNumbers, 0);
    glutMainLoop();

    delete[] squares;
    delete[] dropPoints;
    delete[] idf;
    delete[] collecters;
    delete[] spliters;
    delete[] families;
    delete[] distributers;
    return 0;
}