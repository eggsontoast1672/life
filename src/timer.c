#include "timer.h"

#include <raylib.h>

Timer timer_new(seconds_t length)
{
    return (Timer){
        .length = length,
        .start_time = 0.0,
    };
}

bool timer_tick(Timer *timer)
{
    const seconds_t current_time = GetTime();

    if (current_time - timer->start_time >= timer->length)
    {
        timer->start_time = current_time;
        return true;
    }
    else
    {
        return false;
    }
}
