#ifndef IDF_H
#define IDF_H

using namespace std;

class Idf
{
    public:
        int damage; // damage of the missile 
        int reloadTime; // time need to refill the launcher

    
         Idf(int damage,int reloadTime ){ // constructor for the plane
        
            this->damage = damage;
            this->reloadTime = reloadTime;

    }
    


    void printDetails() {
      
        cout << "Damage of Missile: " <<  this->damage  << endl;
        cout << "Reload Time of Launcher: " << this->reloadTime << endl;
    }
};
#endif