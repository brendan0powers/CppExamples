#include <iostream>
#include "ducklikeobject.h"

using namespace std;

//A class that acts like a duck
class Duck {
public:
    void quack()
    {
        cout << "Duck quacks" << endl;
    }
};

//A class that also acts like a duck
class Person {
public:
    void quack()
    {
        cout << "Person pretends to quack like a duck" << endl;
    }
};

class Chair {
public:
    void doesNothing()
    {
        cout << "Chair continues to do nothing" << endl;
    }
};

void quack(DuckLikeObject duck)
{
    duck.quack();
}

int main()
{
    cout << "Let's quack!" << endl;

    Duck d;
    Person p;
    Chair c;

    quack(d);
    quack(p);

    //This will cause a compile error complaining that Chair has no quack() function;
    //quack(c);

    return 0;
}

