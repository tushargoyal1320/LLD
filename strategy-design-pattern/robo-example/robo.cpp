#include<bits/stdc++.h>

using namespace std;

// Strategy interface for Talkable Robo
class Talkable {
public:
    virtual void talk() = 0;
    virtual ~Talkable() {} //safe to have although not implemented, but it is future-proof (if someone adds a destructor in withTalk let's say)
};

//concreate strategy for talkable
class withTalk : public Talkable {
public:
    void talk() override {
        cout<<"I can talk"<<endl;
    }
};

class withoutTalk : public Talkable{
public:
    void talk() override {
        cout<<"I cannot talk"<<endl;
    }
};

// Strategy interface for Walkable Robo
class Walkable {
public:
    virtual void walk()=0;
    virtual ~Walkable(){}
};

//concreate strategy for walkable
class withWalk : public Walkable {
public:
    void walk() override{
        cout<<"I can walk"<<endl;
    }
};

class withoutWalk : public Walkable {
public:
    void walk() override {
        cout<<"I cannot walk"<<endl;
    }
};


// Strategy interface for Flyable Robo
class Flyable {
public:
    virtual void fly()=0;
    virtual ~Flyable(){}
};

//concreate strategy for flyable
class withFly : public Flyable{
public:
    void fly() override {
        cout<<"I can fly"<<endl;
    }
};

class withoutFly : public Flyable {
public:
    void fly() override{
        cout<<"I cannot fly"<<endl;
    }
};


class Robot{
public:
    Talkable *talkableBehaviour;
    Walkable *walkableBehaviour;
    Flyable *flyableBehaviour;

    Robot(Talkable *talkable, Walkable *walkable, Flyable *flyable) : talkableBehaviour(talkable),walkableBehaviour(walkable),flyableBehaviour(flyable){}

    virtual ~Robot(){
        delete talkableBehaviour;
        delete walkableBehaviour;
        delete flyableBehaviour;
    }

    void walk(){
        walkableBehaviour->walk();
    }

    void talk(){
        talkableBehaviour->talk();
    }

    void fly(){
        flyableBehaviour->fly();
    }
    
    virtual void getVisuals() = 0;
};

class CompanionRobot : public Robot {
public:
    // CompanionRobot(Talkable *talkable, Walkable *walkable, Flyable *flyable){
    //     this->talkableBehaviour = talkable;
    //     this->walkableBehaviour = walkable;
    //     this->flyableBehaviour = flyable;
    // }-> this will give error as there's no default constructor in the Robot class and before entrying the Constructor block
    // it should have a defualt instance, so

    CompanionRobot(Talkable *talkable, Walkable *walkable, Flyable *flyable) : Robot(talkable,walkable,flyable){}

    void getVisuals(){
        cout<<"I'm a companion-robo"<<endl;
    }
};

class WorkerRobot : public Robot{
public:
    WorkerRobot(Talkable *talkable, Walkable *walkable, Flyable *flyable) : Robot(talkable,walkable,flyable) {}

    void getVisuals(){
        cout<<"I'm a worker-robo"<<endl;
    }
};

int main(){
    Robot *robo1 = new CompanionRobot(new withTalk(), new withWalk(), new withFly());
    robo1->talk();
    robo1->fly();
    robo1->walk();
    robo1->getVisuals();

    Robot *robo2 = new WorkerRobot(new withoutTalk(),new withWalk(),new withFly());

    robo2->talk();
    robo2->fly();
    robo2->walk();
    robo2->getVisuals();
    delete robo1;
    delete robo2;
    return 0;
}
