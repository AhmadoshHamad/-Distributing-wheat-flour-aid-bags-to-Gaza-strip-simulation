#ifndef PLANE_H
#define PLANE_H

#include "headers/File.h"
using namespace std;

class Plane
{
    public:
        int refillTime ; // refill time
        int altidute; // the distance above the ground
        int containers; // number of wheat bags
        int dropTime; // the dropping time

    Plane(){ // 
        refillTime =0;
        altidute =0;
        containers =readFromFile("wheatBagsCount=");
        dropTime =0;
    }



    Plane(int refillTime,int altidute,int containers,int dropTime ){ // constructor for the plane
        this->refillTime = refillTime;
        this->altidute = altidute;
        this->containers = containers;
        this->dropTime = dropTime;

    }


    void printDetails() {
        cout << "Refill Time: " << refillTime << " minutes" << std::endl;
        cout << "Altitude: " <<  this->altidute << " meters" << std::endl;
        cout << "Containers: " << containers << std::endl;
        cout << "Drop Time: " << dropTime << " seconds\n" << std::endl;
    }
};
#endif