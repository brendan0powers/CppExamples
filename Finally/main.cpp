#include <iostream>
#include <functional>
#include <stdexcept>
#include <random>
using namespace std;

//Finally class: Takes a std::function and runs it when the class is destroyed
//You can use this to always ensure a bit of code is run when a function or scope
//ends.
class Finally {
public:
    Finally(const function<void()> &func) : m_func(func) {}
    Finally(const Finally &other) = delete; //No copy constructor, function would execute twice!

    ~Finally()
    {
        m_func();
    }

private:
    function<void()> m_func;
};

void testFunc()
{
    random_device::result_type rtRandomNum; //Random number we will use later

    //Create a finally object on the stack
    //The code in the lambda will always execute when the function returns.
    //Variables declared before this function can be accessed and should still be valid.
    Finally f([&]() {
        cout << "This will be called when the function exits! " << rtRandomNum << endl;
    });

    //Generate random numbuer
    random_device rd;
    auto midPoint = ((random_device::max() - random_device::min()) / 2) + random_device::min();
    rtRandomNum = rd();

    //50% change of each outcome
    //The destructor of Finally runs regardless of how the function exits
    if(rtRandomNum < midPoint)
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

