#ifndef LIFE_TIMER_H
#define LIFE_TIMER_H

typedef unsigned int fps_t;
typedef double seconds_t;

typedef struct
{
    seconds_t target_frame_time;
    seconds_t frame_start_time;
} Timer;

Timer timer_new(fps_t target_fps);
void timer_start_frame(Timer *timer);
void timer_end_frame(Timer *timer);

#endif
