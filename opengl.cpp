#include <GL/glut.h>
#include <string>
#include <iostream>
#include <ctime>
#include <algorithm> 

using namespace std;
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

float squareWidth = 0.1f;
float squareHeight = 0.1f;
float spacing = 0.1f;
int numSquares = 5;
int numidf =5;
int numShots;
int shotInterval;
int damageRate;
int FamiliesNum= 0;
int shotsCount = 0;
int numcoll=2;
int spliternum=2;
int distributersNum=2;
int timer2=0;

Square *squares;
DropPoint *dropPoints;
IDF *idf;
Collecters *collecters;
Spliters *spliters;
Families *families;
Distributers*distributers;
int maxDropPoints = 100;

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
    shotsCount++;
    for (int i = 0; i < numidf; ++i) {
        if (shotsCount == idf[i].shotInterval) {
            shotsCount = 0;
            sort(dropPoints, dropPoints + maxDropPoints, [](const DropPoint& a, const DropPoint& b) {
                return a.y > b.y;
            });
            for (int j = 0; j < idf[i].shots; ++j) {
                if (j < maxDropPoints && dropPoints[j].active) {
                        if (dropPoints[j].health!=0){
                        dropPoints[j].health -= idf[i].damageRate;
                        if (dropPoints[j].health <= 0) {
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
                    dropPoints[j].health = 100; 
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
        dropPoints[i].health = 100; 
    }
    for (int i = 0; i < numidf; ++i) {
       idf[i].shots=numShots+i;
       idf[i].shotInterval=shotInterval;
       idf[i].damageRate=damageRate;
    }
    float spacing = 0.05f;
     for (int i = 0; i < numcoll; ++i) {
       collecters[i].health=100;
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
    for (int i = 0; i< FamiliesNum; i++)
    {
        families[i].starvation = rand() % 11 + 10; 
    }
    for(int i=0; i<distributersNum;i++){
        distributers[i].bags=10;
        distributers[i].health=100;
        distributers[i].y =-1.0f;
        distributers[i].x = -i * (squareWidth - 0.05f + spacing );
        distributers[i].orginalX = (-i * (squareWidth - 0.05f + spacing ))-0.1f;
        distributers[i].haveBags=false;
    }
    
}
void display() {
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
    glRasterPos2f(-1.0f, -0.5f); 
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
                        collecters[k].containers+=dropPoints[i].health;
                        collecters[k].health-=5;
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
            if(spliters[j].weight>distributers[i].bags && spliters[j].timer==3 && distributers[i].haveBags==false ){
                spliters[j].weight-=distributers[i].bags;
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
            {   if(families[mostStarvationIndex].starvation>=20){
                   families[mostStarvationIndex].starvation-=20;
                }
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
                                    if(spliters[i].timer!=3){
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
        if (timer2==5){
        families[i].starvation+=1;
        families[i+1].starvation+=1;
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
        if(shotsCount==idf[i].shotInterval-1){
            drawSquare(x, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 0.0f, 0.0f);
        }else{
            drawSquare(x, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 1.0f, 0.0f);
        }
        if(i+1 == numidf)
            break;
        if(shotsCount==idf[i+1].shotInterval-1){
              drawSquare(x-0.08f, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 0.0f, 0.0f);
        }else{
            drawSquare(x-0.08f, y, squareWidth -0.05f, squareHeight-0.05f, 1.0f, 1.0f, 0.0f);
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
    glutSwapBuffers();
}
int main(int argc, char** argv) {
    if (argc ==10 ) {
        numSquares =  stoi(argv[1]);
    } else {
        cout << "Usage: " << argv[0] << " <number_of_squares>" << endl;
        return 1;
    }
    

    numShots = atoi(argv[2]);
    shotInterval = atoi(argv[3]);
    damageRate = atoi(argv[4]);
    numidf =atoi(argv[5]);
    FamiliesNum = atoi(argv[6]);
    numcoll=atoi(argv[7]);
    spliternum=atoi(argv[8]);
    distributersNum=atoi(argv[9]);

   

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
