#include<bits/stdc++.h>

using namespace std;

class Burger{
public:
    virtual void prepare() = 0;
    virtual ~Burger(){}
};

class BasicBurger : public Burger {
public:
    void prepare(){
        cout<<"Preparing Burger with patty, bun and veggies"<<endl;
    }
};

class StandardBurger : public Burger {
public:
    void prepare(){
        cout<<"Preparing Burger with patty, bun, veggies, cheese and lettuce"<<endl;
    }
};

class PremiumBurger : public Burger {
public:
    void prepare(){
        cout<<"Preparing a Gourmet Burger"<<endl;
    }
};

class BurgerFactory {
public:
    Burger* CreateBurger(string type){
        if(type=="Basic")return new BasicBurger();
        else if(type=="Standard")return new StandardBurger();
        else if(type=="Premium")return new PremiumBurger();
        else{
            cout<<"Invalid Burger type"<<endl;
            return NULL;
        }
    }
};

int main(){
    BurgerFactory* myBurgerFactory = new BurgerFactory();

    Burger* myBurger = myBurgerFactory->CreateBurger("Premium");
    myBurger->prepare();


}