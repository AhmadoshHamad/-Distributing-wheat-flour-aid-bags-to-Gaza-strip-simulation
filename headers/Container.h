#ifndef Container_H
#define Container_H

#define FALLING     1
#define LANDED      2 
#define BLOWN       3
#define COLLECTED   4


class Container
{
    public:
        int altidute; // the distance above the ground
        int weight; // the weight of the container so the splitter will split them in 1 kilos bags
        int status; // the status of the container aka if its in the falling or its arrived the ground
        
    
         Container(int altidute,int weight,int status ){ // constructor for the plane
        
            this->altidute = altidute;
            this->weight = weight;
            this->status = status;

    }

       


    void printDetails() {
        
        cout << "Altitude: " <<  this->altidute << " meters" << endl;
        cout << "Container Weight: " << this->weight  << endl;
        cout << "Container Status: " << this->status  << endl << endl ;
    }

    void decreaseAltitude(){
        this->altidute-= 2;
        if(this->altidute <= 0 )
            this->status = LANDED;
        cout << "decreasing altitude\n";
        sleep(1);
    }


    void damage(){
        this->weight-= 5;
        if(this->weight <= 0){
            this->status = BLOWN;
            cout << "!!!! Container BLOWN !!!!!!!" <<endl;
        }
        cout << this->altidute << endl;
        sleep(1);

    }
    



};

#endif