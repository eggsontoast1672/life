#include "timer.h"

#include <raylib.h>

Timer timer_new(fps_t target_fps)
{
    return (Timer){
        .target_frame_time = 1.0 / target_fps,
    };
}

void timer_start_frame(Timer *timer)
{
    timer->frame_start_time = GetTime();
}

void timer_end_frame(Timer *timer)
{
    const seconds_t frame_end_time = GetTime();
    const seconds_t frame_time = frame_end_time - timer->frame_start_time;
    if (frame_time < timer->target_frame_time)
    {
        const seconds_t difference = timer->target_frame_time - frame_time;
        WaitTime(difference);
    }
}
