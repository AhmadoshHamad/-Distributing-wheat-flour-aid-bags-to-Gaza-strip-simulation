#ifndef PLANE_H
#define PLANE_H

class Plane
{
    public:
        int refillTime ; // refill time
        int altidute; // the distance above the ground
        int containers; // number of wheat bags
        int tof; // the time of flight

    Plane(int refillTime,int altidute,int containers,int tof ){ // constructor for the plane
        this->refillTime = refillTime;
        this->altidute = altidute;
        this->containers = containers;
        this->tof = tof;

    }
};
#endif