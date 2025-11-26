#include<bits/stdc++.h>

using namespace std;

class Vehicle {
    string number;
    string type;
public: 
    Vehicle(string n,string t): number(n),type(t){}
    string getType(){
        return type;
    }
    string getNumber(){
        return number;
    }
};

class Spot{
    int id;
    int floor_id;
    string type;
    string status;
public:
    Spot(int id_,int floor,string t) : id(id_),floor_id(floor),type(t),status("AVAILABLE"){}
    bool isFree(){
        return status == "AVAILABLE";
    }

    string getType(){
        return type;
    }

    int getId(){
        return id;
    }
    //park
    int park(Vehicle& v){
        status = "OCCUPIED";
        return 1;
    }
    
    //unpark
    int unpark(){
        status = "AVAILABLE";
        return 1;
    }
    
    //canFit
};

// parking lot

// floor
class Floor{
    int id;
    vector<Spot*> spots;
public:
    Floor(int id_,int twoWheeler, int fourWheeler,int truck,int spot_start) : id(id_){
        int spot_id = spot_start;
        Spot *spot;
        for(int i=0;i<twoWheeler;i++){
            spot = new Spot(spot_id,id_,"twoWheeler");
            spots.push_back(spot);
            spot_id++;
        }
        for(int i=0;i<fourWheeler;i++){
            spot = new Spot(spot_id,id_,"fourWheeler");
            spots.push_back(spot);
            spot_id++;
        }
        for(int i=0;i<truck;i++){
            spot = new Spot(spot_id,id_,"truck");
            spots.push_back(spot);
            spot_id++;
        } 
    }
    vector<Spot*>& getSpots(){
        return spots;
    }

    //find free spot
    Spot* findSpot(string type){
        for(auto &spot : spots){
            if(spot->isFree() && spot->getType() == type){
                return spot;
            }
        }
        return NULL;
    }
};

// Ticket
class Ticket{
    public:
    int id;
    time_t entry;
    string vehicle_number;
    Spot* spot;
    //issue ticket
    Ticket(string n,Spot *s) : vehicle_number(n),spot(s){
        id = rand();
        entry = time(NULL);
    }
    //calculate fare
    double calculateFare(time_t entry,time_t exit){
        double hours = difftime(exit, entry)/3600.0;
        return hours*20.0;
    }

};

class ParkingLot {
    vector<Floor*> floors;
    unordered_map<int,Ticket*> activeTickets;
public:
    ParkingLot(vector<Floor*> f) : floors(f){

    }
    Ticket* EnterCar(Vehicle& v){
        //find spot
        Spot *spot = NULL;
        for(auto floor : floors){
            spot = floor->findSpot(v.getType());
            if(spot){
                break;
            }
        }

        // create ticket
        if(spot==NULL)return NULL;
        spot->park(v);

        Ticket *ticket = new Ticket(v.getNumber(),spot);
        activeTickets[ticket->id] = ticket;
        cout<<"Car parked at"<<spot->getId()<<"ticket:"<<ticket->id<<endl;
        return ticket;
    }

    double ExitCar(Ticket *t){
        time_t exit_time = time(NULL);
        double fare = t->calculateFare(t->entry,exit_time);

        t->spot->unpark();
        activeTickets.erase(t->id);
        cout<<"Car exiting the parking, pay the amount : "<<fare<<endl;
        return fare;
    }
};

int main(){
    
    Floor *f1 = new Floor(1,2,4,3,1);
    Floor *f2 = new Floor(2,4,6,1,10);
    vector<Floor*> floors = {f1,f2};
    ParkingLot* lot = new ParkingLot(floors);

    Vehicle *v = new Vehicle("UP16CP4001","twoWheeler");
    Ticket *t = lot->EnterCar(*v);
    if(t) cout << "Ticket issued: " << t->id << "\n";
    else cout << "Parking failed\n";

    cout<<lot->ExitCar(t);

}