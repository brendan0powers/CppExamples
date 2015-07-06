#include <iostream>
#include "units.h"

using namespace std;

int main()
{
    //This would give a compile error
    //what is 50? Seconds? Minutes?
    //timeSleep(50);

    //This works
    timeSleep(50_ms); //Sleeps for 50ms

    //This also works
    timeSleep(10_s); //Sleeps for 10000ms

    //This should work too
    timeSleep(0.5_m); //Sleep for 30000ms

    //Unit conversions

    cout << "3 seconds to ms " << (3_s).ms() << endl;
    cout << "20 hours to days " << (20_h).d() << endl;

    return 0;
}

