#ifndef PLANE_H
#define PLANE_H

class Plane
{
    public:
        int refillTime ; // refill time
        int altitude; // the distance above the ground
        int containers; // number of wheat bags
        int dropTime; // the time of flight

    Plane(int refillTime,int altitude,int containers,int dropTime){ // constructor for the plane
        this->refillTime = refillTime;
        this->altitude = altitude;
        this->containers = containers;
        this->dropTime = dropTime;
    }
    void printDetails() {
        std::cout << "Refill Time: " << refillTime << " minutes" << std::endl;
        std::cout << "Altitude: " << altitude << " meters" << std::endl;
        std::cout << "Containers: " << containers << std::endl;
        std::cout << "Drop Time: " << dropTime << " seconds\n" << std::endl;
    }
};
#endif
