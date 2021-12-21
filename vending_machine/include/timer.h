#pragma once

#include <time.h>

class Timer
{
private:
    time_t start_time;
    int duration;
    bool active;

public:
    Timer();
    void Set(int duration);
    bool GetTimedOut();
};