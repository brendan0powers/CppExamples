#ifndef TIME_H
#define TIME_H

#include <iostream>

class Time
{
public:
    //Define, tags for creation
    class NanoSecond {};
    class MicroSecond {};
    class MilliSecond {};
    class Second {};
    class Minute {};
    class Hour {};
    class Day {};

    //Tagged constructors
    inline constexpr Time() : m_dTimeSec(0) {}
    inline constexpr Time(const NanoSecond, const double dTime) : m_dTimeSec(dTime/(1000 * 3)) {}
    inline constexpr Time(const MicroSecond, const double dTime) : m_dTimeSec(dTime/(1000 * 2)) {}
    inline constexpr Time(const MilliSecond, const double dTime) : m_dTimeSec(dTime/(1000 * 1)) {}
    inline constexpr Time(const Second, const double dTime) : m_dTimeSec(dTime) {}
    inline constexpr Time(const Minute, const double dTime) : m_dTimeSec(dTime*60) {}
    inline constexpr Time(const Hour, const double dTime) : m_dTimeSec(dTime*3600) {}
    inline constexpr Time(const Day, const double dTime) : m_dTimeSec(dTime*3600*24) {}

    inline constexpr double ns() const { return m_dTimeSec * 1000 * 3; }
    inline constexpr double us() const { return m_dTimeSec * 1000 * 2; }
    inline constexpr double ms() const { return m_dTimeSec * 1000 * 1; }
    inline constexpr double s() const { return m_dTimeSec; }
    inline constexpr double m() const { return m_dTimeSec / 60; }
    inline constexpr double h() const { return m_dTimeSec / 3600; }
    inline constexpr double d() const { return m_dTimeSec / (3600 * 24); }
private:
    const double m_dTimeSec;
};

//Double
inline constexpr Time operator "" _ns(long double dTime) { return Time(Time::NanoSecond(), dTime); }
inline constexpr Time operator "" _us(long double dTime) { return Time(Time::MicroSecond(), dTime); }
inline constexpr Time operator "" _ms(long double dTime) { return Time(Time::MilliSecond(), dTime); }
inline constexpr Time operator "" _s(long double dTime) { return Time(Time::Second(), dTime); }
inline constexpr Time operator "" _m(long double dTime) { return Time(Time::Minute(), dTime); }
inline constexpr Time operator "" _h(long double dTime) { return Time(Time::Hour(), dTime); }
inline constexpr Time operator "" _d(long double dTime) { return Time(Time::Day(), dTime); }

//Integer
inline constexpr Time operator "" _ns(unsigned long long int dTime) { return Time(Time::NanoSecond(), dTime); }
inline constexpr Time operator "" _us(unsigned long long int dTime) { return Time(Time::MicroSecond(), dTime); }
inline constexpr Time operator "" _ms(unsigned long long int dTime) { return Time(Time::MilliSecond(), dTime); }
inline constexpr Time operator "" _s(unsigned long long int dTime) { return Time(Time::Second(), dTime); }
inline constexpr Time operator "" _m(unsigned long long int dTime) { return Time(Time::Minute(), dTime); }
inline constexpr Time operator "" _h(unsigned long long int dTime) { return Time(Time::Hour(), dTime); }
inline constexpr Time operator "" _d(unsigned long long int dTime) { return Time(Time::Day(), dTime); }

//A sleep function that takes a time object
void timeSleep(const Time t)
{
    std::cout << "Sleeping for " << t.ms() << "ms" << std::endl;
}

#endif // TIME_H
