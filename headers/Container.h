#ifndef Container_H
#define Container_H

using namespace std;

class Container
{
    public:
        int altidute; // the distance above the ground
        int weight; // the weight of the container so the splitter will split them in 1 kilos bags

    
         Container(int altidute,int weight ){ // constructor for the plane
        
            this->altidute = altidute;
            this->weight = weight;

    }
    


    void printDetails() {
      
        cout << "Altitude: " <<  this->altidute << " meters" << endl;
        cout << "Container Weight: " << this->weight << " \n" << endl;
    }
};
#endif