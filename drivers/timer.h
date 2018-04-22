#ifndef __TIMER_H__
#define __TIMER_H__

#include <types.h>

#define SYSTIME_FREQUENCY 1000
#define SYSTIME_NB_TIMER  8

#define TICKS_OF_SEC(seconds) ((uint32_t)(seconds * systime.ticksPerSec))

typedef struct{

    void (*callback) (void);

    volatile bool elapsed;
    uint32_t      endTime ;     ///< in SYS_TIME_TICKS
    uint32_t      duration;     ///< in SYS_TIME_TICKS

}TIMER;

typedef struct
{
    volatile uint32_t nbSec;       ///< full seconds since startup
    volatile uint32_t nbSecRem;    ///< remainder of seconds since startup in CPU_TICKS
    volatile uint32_t nbTicks;     ///< SYS_TIME_TICKS since startup
    uint8_t  nbTimer;
    float    resolution;            ///< sys_time_timer resolution in seconds
    uint32_t ticksPerSec;           ///< sys_time ticks per second (SYS_TIME_FREQUENCY)
    uint32_t resolutionCPUTicks;    ///< sys_time_timer resolution in cpu ticks
    uint32_t cpuTicksPerSec;        ///< cpu ticks per second

    TIMER *  timers[SYSTIME_NB_TIMER];
}SYSTIME;

void TIMER_Init (TIMER *, float duration, void (*callback)(void));

bool TIMER_Scutation (TIMER *);

#endif /* __TIMER_H__ */