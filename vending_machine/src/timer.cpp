#include <time.h>
#include "timer.h"

// debugging:
#include <iostream>

Timer::Timer()
{
    this->duration = 0;
    this->start_time = clock();
    this->active = false;
}

void Timer::Set(int duration)
{
    this->start_time = time(NULL);
    this->duration = duration;
    this->active = true;
}

bool Timer::GetTimedOut()
{
    time_t current_time = time(NULL);
    int elapsed = (current_time - start_time); 
    bool result = active && (elapsed >= this->duration);
    if (result)
    {
        this->active = false;
    }
    return result;
}
