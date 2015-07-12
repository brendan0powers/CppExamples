#include <iostream>
#include <functional>
#include <stdexcept>
#include <random>
using namespace std;

//ScopedLambda class: Takes a reference to a lambda and runs it when the class is destroyed
//You can use this to always ensure a bit of code is run when a function or scope ends.
template <class Lambda>
class ScopedLambda {
public:
    ScopedLambda(const Lambda &func) : m_func(func) {}
    ScopedLambda(const ScopedLambda &other) = delete; //No copy constructor, function would execute twice!

    ~ScopedLambda()
    {
        m_func();
    }

private:
    const Lambda& m_func;
};

//Some Macro magic here to make the Finally() macro work
#define FINALLY_CONCAT_PAIR_INNER(x,y) x ## y
#define FINALLY_CONCAT_PAIR(x,y) FINALLY_CONCAT_PAIR_INNER(x,y)

#define FINALLY_INNER2(lambda, object, ...) auto lambda = [&]() { __VA_ARGS__ ;}; ScopedLambda<decltype(lambda)> object(lambda);
#define FINALLY_INNER(unique, ...) FINALLY_INNER2(FINALLY_CONCAT_PAIR(finallyLambda, unique), \
    FINALLY_CONCAT_PAIR(finallyObject, unique), __VA_ARGS__)

//The Finally() macro does the real magic here. It constructs a lambda and ScopedLambda object from the arguments passed to it
#define FINALLY(...) FINALLY_INNER(123, __VA_ARGS__)

//50/50 chance of returning true/false
bool getRandomBool();

void testFunc()
{
    bool bRandom = false;  //A variable declared before the FINALLY can be used

    //Run the code in FINALLY() at the end of the function. The FINALLY macro creates
    //a lambda from the specified block, along with a ScopedLambda object. When the
    //ScopedLambda object goes out of scope, the lambda will execute.
    //Variables declared before this macro can be accessed and should still be valid.
    FINALLY(cout << "This will be called when the function exits! " << bRandom << endl;);

    //Assign a random value to the variable
    //Code in the FINALLY block will have access to the new value
    bRandom = getRandomBool();

    //50% change of each outcome
    //The destructor of Finally runs regardless of how the function exits
    if(bRandom)
    {
        cout << "Returning Normally" << endl;
        return;
    }
    else
    {
        cout << "Throwing Exception" << endl;
        throw runtime_error("Exception happened.");
    }
}

int main()
{
    try {
        testFunc();
        cout << "Function succeeded" << endl;
    }
    catch(const exception &e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}


//not importatnt to the example
bool getRandomBool()
{
    //Generate random numbuer
    random_device rd;

    //Figure out that the 'mid point' of our range of random numbers is
    auto midPoint = ((random_device::max() - random_device::min()) / 2) + random_device::min();

    return (rd() < midPoint); //is the random number less than the midpoint?
}
