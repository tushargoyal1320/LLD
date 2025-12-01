#include<bits/stdc++.h>

using namespace std;

enum class VehicleType {CAR, TRUCK, BIKE};
enum class ParkingSpotType {CAR, TRUCK, BIKE};

class Vehicle {
protected:
    string licence_plate;
    VehicleType type;
    Vehicle(string n,VehicleType t): licence_plate(n),type(t){}
public:  
    VehicleType getType(){
        return type;
    }
    string getNumber(){
        return licence_plate;
    }
};

class Car : public Vehicle{
public:
    Car(string lp) : Vehicle(lp,VehicleType::CAR){}
};

class Bike : public Vehicle{
public:
    Bike(string lp) : Vehicle(lp,VehicleType::BIKE){}
};

class ParkingSpot{
    int id;
    int floor_id;
    ParkingSpotType type;
    string status;
    mutex mtx;
    Vehicle *parked_vehicle = nullptr; // Initialize parked_vehicle to nullptr
public:
    ParkingSpot(int id_,int floor,ParkingSpotType t) : id(id_),floor_id(floor),type(t),status("AVAILABLE"){}
    bool isFree(){
        return status == "AVAILABLE";
    }

    ParkingSpotType getType(){
        return type;
    }

    int getId(){
        return id;
    }
    //park
    bool assignVehicle(Vehicle& v){
        lock_guard<mutex> lock(mtx);
        if(!isFree())return false;
        parked_vehicle = &v;
        status = "OCCUPIED";
        return true;
    }
    
    //unpark
    void removeVehicle(){
        lock_guard<mutex> lock(mtx);
        parked_vehicle = NULL;
        status = "AVAILABLE";
    }

    //canFit
    bool canFitVehicle(Vehicle *v) {
        return static_cast<int>(v->getType()) == static_cast<int>(this->type);
    }
};

// Ticket
class Ticket{
    public:
    int id;
    time_t entry;
    string vehicle_number;
    ParkingSpot* spot;
    //issue ticket
    Ticket(string n,ParkingSpot *s) : vehicle_number(n),spot(s){
        id = rand();
        entry = time(NULL);
    }
    //calculate fare
    double calculateFare(time_t exit){
        double hours = difftime(exit, this->entry)/3600.0;
        return ceil(hours)*20.0;
    }

};

// floor
class Level{
    int level_number;
    vector<ParkingSpot*> spots;
public:
    Level(int l,int twoWheeler, int fourWheeler,int truck,int spot_start) : level_number(l){ 
        // twoWheeler -> BIKE, fourWheeler -> CAR, truck -> TRUCK
        int spot_id = spot_start;
        while (twoWheeler--)  spots.push_back(new ParkingSpot(spot_id++, l, ParkingSpotType::BIKE));
        while (fourWheeler--) spots.push_back(new ParkingSpot(spot_id++, l, ParkingSpotType::CAR));
        while (truck--)       spots.push_back(new ParkingSpot(spot_id++, l, ParkingSpotType::TRUCK));
    }

    ~Level(){
        for(auto spot : spots){
            delete spot;
        }
    }

    Ticket* parkVehicle(Vehicle* v) {
        for(auto spot : spots){
            if(spot->isFree() && spot->canFitVehicle(v)){
                spot->assignVehicle(*v);
                Ticket* ticket = new Ticket(v->getNumber(),spot);
                return ticket;
            }
        }
        return NULL;
    }
};

class ParkingStrategy {
public:
    virtual Ticket* assign(vector<Level*> &levels, Vehicle *v) = 0;
    virtual ~ParkingStrategy() {}
};

class NearestSpotStrategy : public ParkingStrategy {
public:
    Ticket* assign(vector<Level*> &levels, Vehicle *v) override{
        for(auto l : levels){
            Ticket *t = l->parkVehicle(v);
            if(t) return t;
        }
        return NULL;
    }
};

class PaymentStrategy {
    public:
    virtual double calculateFare(time_t entry,time_t exit) = 0;
    virtual ~PaymentStrategy() {}
};

class payByHour : public PaymentStrategy {
public:
    double calculateFare(time_t entry,time_t exit) override {
        int hours = ceil(difftime(exit,entry)/3600.0);
        return hours*20.0;
    }
};

class FlatPricing : public PaymentStrategy {
public:
    double calculateFare(time_t entry,time_t exit) override {
        return 50.0;
    }
};

class ParkingLot {
    vector<Level*> levels;
    static ParkingLot* instance;
    static mutex mtx;
    ParkingStrategy* parkingStrategy;
    PaymentStrategy* paymentStrategy;
    unordered_map<int,Ticket*> activeTickets; // {ticket_id, Ticket*}

    ParkingLot() {
        parkingStrategy = new NearestSpotStrategy();
        paymentStrategy = new FlatPricing();
    }

public:
    static ParkingLot* getInstance(){
        if(!instance){
            lock_guard<mutex> lock(mtx);
            if(!instance)
                instance = new ParkingLot();
        }
        return instance;
    }

    void setParkingStrategy(PaymentStrategy *p){
        paymentStrategy = p;
    }

    void setParkingStrategy(ParkingStrategy *p){
        parkingStrategy = p;
    }

    // allow adding levels to the lot
    void addLevel(Level* l){
        levels.push_back(l);
    }

    Ticket* ParkVehicle(Vehicle *v){
        Ticket *t = parkingStrategy->assign(levels,v);
        if(t) {
            activeTickets[t->id] = t;
        }
        return t;
    }

    double unparkVehicle(int ticketId){
        if(!activeTickets.count(ticketId))return -1;

        Ticket *t = activeTickets[ticketId];

        t->spot->removeVehicle();

        double fare = paymentStrategy->calculateFare(t->entry,time(NULL));

        delete t;

        activeTickets.erase(ticketId);

        return fare;
    }

    ~ParkingLot() {
        for (auto lvl : levels) delete lvl;
        for (auto p : activeTickets) delete p.second;
        delete parkingStrategy;
        delete paymentStrategy;
    }
};

ParkingLot* ParkingLot::instance = nullptr;
mutex ParkingLot::mtx;

int main(){
    Level *f1 = new Level(1,2,4,3,1);
    Level *f2 = new Level(2,4,6,1,10);
    
    ParkingLot* lot = ParkingLot::getInstance();
    // register levels with parking lot
    lot->addLevel(f1);
    lot->addLevel(f2);

    Vehicle *v = new Bike("UP16CP4001");
    Ticket * t = lot->ParkVehicle(v);
    if(!t){
        cerr << "Parking failed - no available spots" << endl;
        return 1;
    }

    cout << "Ticket id: " << t->id << "\n";
    cout << lot->unparkVehicle(t->id) << "\n";
    delete v;
    return 0;
}