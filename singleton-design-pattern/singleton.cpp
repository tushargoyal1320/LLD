#include<bits/stdc++.h>

using namespace std;

class Singleton {
private:
    static Singleton* instance;
    static mutex mtx;
    
    // made constructor private to prevent outside instantiation.
    Singleton(){
        cout<<"Creating a new object"<<endl;
    }

    // Prevent copying
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
public:
    static Singleton* getInstance() {
        if(instance==NULL){
            lock_guard<mutex> lock(mtx);
            if(instance==NULL)
                instance = new Singleton();
            /*
                Double locking so that if two threads aquire the lock at the same time,
                let's say t1 acquires the lock first it'll instanciate the instance
                after unlock, t2 also tries to create a new instance, which we don't want 
                so we want to be double sure.
            */
        }
        return instance;
    }
};

//initialize

Singleton* Singleton:: instance = NULL;
mutex Singleton::mtx;

int main(){
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    if(s1==s2){
        cout<<"Same!"<<endl;
    }else{
        cout<<"Different"<<endl;
    }
}
