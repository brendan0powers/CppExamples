# CppExamples
Practical examples for new C++ features

### Finally
An implementation of the finally keywork in C++. The idea behind a finally keyword
is to ensure a piece of code runs when a function exits. This is important with 
languages that support exceptions. Since any function you call may throw an exception,
you must assume that your function could exit at any time. If your function exits, you must
ensure you've cleaned up any resources (locks, files, DB transactions) you allocated at the
start of the function. This problem is solved in c++ with the RAII pattern. However, the RAII 
pattern requires you to have a class writen for the specific task you are doing. In some 
cases it does not make sense to write an entire new class just to clean up a one-off resource. 
In this case, a finally keyword would be usefull.

In the Finally example, I have implement a finally keyword using a combination of a macro, and
a lambda function. the FINALLY macro declares a class named ScopedLambda. The ScopedLambda
class stores the lambda function that runs when the class goes out of scope. This means that
any code contained in the FINALLY macro will run when the function exits. Regardless of the
reason or time of exit.

Here's an example of a function that prints a message when the function exits. In this case,
an exception is thrown.

```C++
void testFunc()
{
  FINALLY(cout() << "This text will print when the function ends." << endl;);
  
  throw runtime_error("Some exception happened");
}
```

A simpler implementation of the finally concept would be to just use the ScopedLambda
class, and leave it's set up to the user. Here is an equivelent example to the above.

```C++
void testFunc()
{
  ScopedLambda finally([&]() {
    cout() << "This text will print when the function ends." << endl;
  });
  
  throw runtime_error("Some exception happened");
}
```

The macro implentation has some advantages over this example. The most obvious 
being that the user need not declare a variable. It's quite commont to forget to
name the variable when using RAII classes. For example 

```C++
ScopedLambda([&]{cout << "Test;});
```

This cope would create, and immediatly destroy the object, causing the cleanup
code to run early. These sorts of mistakes are usualy easy to catch, but sometimes
cause serous bugs. The other advantage of the macro implementation is less obvoius.
Due to a quirck of C++ lambdas, it is not possible to know their type. This means
you cannot directly store a lambda. The macro-less implementation of ScopedLambda
must use the std::function() object. This object has a fair amount of overhead, and
may not be suitable for performance critical applications. I generally prefer to avoid
premature optimiztion. Avoiding use of the std::function object would seem to meet that
criteria. However, I'd like to use the FINALLY keyword in all parts of the applications I
write, and not worry too much about it's impact on performance.

### Units
The Units example demonstrates how to use user defined literals to handle
numbers in a type safe way. For example, the function below
```C++
sleep(5);
```
is unclear. Without going to the documentation of the function, there is no
way to know how long this function will sleep for. In order to be more clear, we could name the function
`sleepMs(5)`. This solved the ambiguity problem, but there's still no compile time checking that ensure that
when we say 5, we meen 5 milliseconds. For example, the code below shows how the situation can be easilly
confused once again.

```C++
int doWork(std::vector<int> vectWork, int iPauseInterval)
{
  int sum = 0;

  for(auto i, vectWork)
  {
    sum += i;
    sleepMs(iPauseInterval);
  }
  
  return sum;
}
```
Although this is a somewhat contrived example, it illustrates the problem nicely. Even though we have a
sleepMs function, the iPauseInterval parameter does not indicate what unit the pause interval is. We could
clarify the situation a bit by renaming it iPauseIntervalMs. Once again though, this is only apparent if the
end user reads the documentation. 

So, we'd ideally like a solution that uses C++'s type system to do the hard work of ensureing the correct unit
for us. In the Units example, I provide a very basic implementation of a Time class that can be used to solve
this problem. In the example, the sleep function looks something like this.

```C++
void sleep(const Time t)
{
  //Don't actually sleep, but print out what we'd do if this were a real
  //sleep function
  std::cout << "This will sleep for " << t.ms() << "ms" << endl;
}
```

The Time class provides us with several important advantages
1) There is no default constructor for Time that takes an int or a double. Therefore the previous 
  example of `sleep(5)` would generate a compile error.
2) Since there is no constructor from int or double, you must specify exactly what units you
  intend to use when creating a Time object.
3) The Time class internally stores the time value as a double in seconds, and provides conversion
  functions for the varoius time units. .us() .ms(), etc...
4) the constexpr statement can be used to initialize time constants at compile time. This means that
  using the Time class should have no performance penalty as compared to using a double for time storage.

However, initializing the time class is somewhat awkward, and far to verbose to be convenient. Having
to call sleep like this `sleep(Time(Time::Milliseconds, 5));` all the time would quickly become tedious.
To solve this problem, we can use C++11's user defined literals. instead of `Time(Time::Milliseconds, 5)`
to construct a Time object, we can simply use 5_ms. the _ms operator will create a Time object for us.
This allows us to use a streightforward and clear syntax when calling the new sleep function. `sleep(5_ms);`
is quite clear and self documenting. In addition, we can specify different units to the sleep function,
and the Time class will handle the unit conversions for us. For example, asll of the lines below are valid.

```C++
sleep(5_ns); //sleep for 5 nanoseconds
sleep(5_us); //sleep for 5 microseconds
sleep(5_ms); //sleep for 5 milliseconds
sleep(5_s);  //sleep for 5 seconds
sleep(5_m);  //sleep for 5 minutes
sleep(5_h);  //sleep for 5 hours
sleep(5_d);  //sleep for 5 days
```

All this whout having to declare sleepNs() sleepUs(), sleepMs(), etc... The meaning of each line is clear, 
and there is no reason to reference documentaion to determine how the code will behave. If we re-write the
example from above, we can see how the Time class can clarify the doWork() function.

```C++
int doWork(std::vector<int> vectWork, Time tPauseInterval)
{
  int sum = 0;

  for(auto i, vectWork)
  {
    sum += i;
    sleep(tPauseInterval);
  }
  
  return sum;
}
```

You can see from the revised code sample that instead of taking an int iPauseInterval, we now take Time object.
We then pass that object allong to the sleep() function. It is now impossible to pass a unitless integer 
to the doWork function.  For example

```C++
doWork(someVect, 5); //Causes a compile error
doWork(someVect, 5_ms); //waits for 5ms between adding integers
doWork(someVect, 1_m); //waits for 1 minute between adding integers
```

All these are valid ways to call doWork() (except the first). The function itself knows nothing about time uints. 
This also allows the caller of the function to pick the unit of time that is the clearest to read in their use-case.
