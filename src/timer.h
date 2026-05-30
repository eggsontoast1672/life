#ifndef LIFE_TIMER_H
#define LIFE_TIMER_H

#include <stdbool.h>

typedef double seconds_t;

typedef struct
{
    seconds_t length;
    seconds_t start_time;
} Timer;

Timer timer_new(seconds_t length);
bool timer_tick(Timer *timer);

#endif
