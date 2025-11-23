#include<bits/stdc++.h>

using namespace std;

class Burger {
    public:
    virtual void prepare() = 0;
    virtual ~Burger() {};
};

class BasicBurger : public Burger {
    public:
    void prepare() override{
        cout<<"preparing basic burger"<<endl;
    }
};

class StandardBurger : public Burger {
    public:
    void prepare() override{
        cout<<"preparing standard burger"<<endl;
    }
};

class PremiumBurger : public Burger {
    public:
    void prepare() override{
        cout<<"preparing premium burger"<<endl;
    }
};

class BasicWheatBurger : public Burger {
    public:
    void prepare() override{
        cout<<"preparing basic wheat burger"<<endl;
    }
};

class StandardWheatBurger : public Burger {
    public:
    void prepare() override{
        cout<<"preparing standard wheat burger"<<endl;
    }
};

class PremiumWheatBurger : public Burger {
    public:
    void prepare() override{
        cout<<"preparing premium wheat burger"<<endl;
    }
};

class GarlicBread {
    public:
    virtual void prepare() = 0;
    virtual ~GarlicBread() {};
};

class BasicGarlicBread : public GarlicBread {
    public:
    void prepare() override{
        cout<<"preparing basic GarlicBread"<<endl;
    }
};

class StandardGarlicBread : public GarlicBread {
    public:
    void prepare() override{
        cout<<"preparing standard GarlicBread"<<endl;
    }
};

class PremiumGarlicBread : public GarlicBread {
    public:
    void prepare() override{
        cout<<"preparing premium GarlicBread"<<endl;
    }
};

class BasicWheatGarlicBread : public GarlicBread {
    public:
    void prepare() override{
        cout<<"preparing basic wheat GarlicBread"<<endl;
    }
};

class StandardWheatGarlicBread : public GarlicBread {
    public:
    void prepare() override{
        cout<<"preparing standard wheat GarlicBread"<<endl;
    }
};

class PremiumWheatGarlicBread: public GarlicBread {
    public:
    void prepare() override{
        cout<<"preparing premium wheat GarlicBread"<<endl;
    }
};

class BurgerFactory {
public:
    virtual Burger* createBurger(string type) = 0;
    virtual GarlicBread* createGarlicBread(string type) = 0;
};

class SinghBurger : public BurgerFactory{
public:
    Burger* createBurger(string type) override{
        if(type=="basic")return new BasicBurger();
        else if(type=="standard")return new StandardBurger();
        else if(type=="premium")return new PremiumBurger();
        else{
            cout<<"Invalid Type"<<endl;
            return NULL;
        }
    }

    GarlicBread* createGarlicBread(string type) override{
        if(type=="basic")return new BasicGarlicBread();
        else if(type=="standard")return new StandardGarlicBread();
        else if(type=="premium")return new PremiumGarlicBread();
        else{
            cout<<"Invalid Type"<<endl;
            return NULL;
        }
    }
};

class KingBurger : public BurgerFactory{
public:
    Burger* createBurger(string type) override{
        if(type=="basic")return new BasicWheatBurger();
        else if(type=="standard")return new StandardWheatBurger();
        else if(type=="premium")return new PremiumWheatBurger();
        else{
            cout<<"Invalid Type"<<endl;
            return NULL;
        }
    }

    GarlicBread* createGarlicBread(string type) override{
        if(type=="basic")return new BasicWheatGarlicBread();
        else if(type=="standard")return new StandardWheatGarlicBread();
        else if(type=="premium")return new PremiumWheatGarlicBread();
        else{
            cout<<"Invalid Type"<<endl;
            return NULL;
        }
    }
};

int main(){
    BurgerFactory *burgerFactory = new SinghBurger();
    Burger *myBurger = burgerFactory->createBurger("basic");
    GarlicBread *myGarlicBread = burgerFactory->createGarlicBread("premium");
    myBurger->prepare();
    myGarlicBread->prepare();


    burgerFactory = new KingBurger();
    myBurger = burgerFactory->createBurger("basic");
    myGarlicBread = burgerFactory->createGarlicBread("premium");
    myBurger->prepare();
    myGarlicBread->prepare();


    delete myBurger;
    delete myGarlicBread;
}