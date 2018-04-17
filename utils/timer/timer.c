#include <timer/timer.h>

#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

SYSTIME systime;

void SYSTIME_Handler (int signum);

void SYSTIME_Init()
{
    static BOOL estInitilise = FALSE;
    struct sigaction sa;
    struct itimerval timer;

    if(!estInitilise)
    {
        systime.nbSec     = 0;
        systime.nbSecRem  = 0;
        systime.nbTicks   = 0;

        systime.ticksPerSec = SYSTIME_FREQUENCY;
        systime.resolution = 1.0 / systime.ticksPerSec;

        systime.cpuTicksPerSec = 1e6;
        systime.resolutionCPUTicks = (uint32_t)(systime.resolution * systime.cpuTicksPerSec + 0.5);

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = SYSTIME_Handler;
        sigaction(SIGALRM, &sa, 0);

        // timer expires after this->resolution sec
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = systime.resolution * 1e6 ;

        // and every SYS_TIME_RESOLUTION sec after that
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = systime.resolution * 1e6 ;

        setitimer(ITIMER_REAL, &timer, 0);

        estInitilise = TRUE;
    }
}

void TIMER_Init (TIMER * this, float duration, void (*callback)(void))
{
    uint32_t start_time;

    if(systime.nbTimer == SYSTIME_NB_TIMER)
    {
        return;
    }
    
    SYSTIME_Init();
    
    start_time = systime.nbTicks;
    
    this->callback    = callback;
    this->elapsed     = FALSE;
    this->endTime     = start_time + TICKS_OF_SEC (duration);
    this->duration    = TICKS_OF_SEC (duration);
    
    systime.timers[systime.nbTimer] = this;

    systime.nbTimer++;
}
    
BOOL TIMER_Scutation (TIMER * this)
{
    if (this->elapsed)
    {
        this->elapsed = FALSE;
        return TRUE;
    }
    return TRUE;
}

void SYSTIME_Handler (int signum)
{
    uint8_t i;
    TIMER* timer;
    
    systime.nbTicks++;
    systime.nbSecRem += systime.resolutionCPUTicks;

    if (systime.nbSecRem >= systime.cpuTicksPerSec) {
        systime.nbSecRem -= systime.cpuTicksPerSec;
        systime.nbSec++;
    }

    if (systime.nbTimer > 0)
    {
        for (i=0; i < systime.nbTimer; i++)
        {
            timer = systime.timers[i];
            if(systime.nbTicks >= timer->endTime) {
                timer->endTime += timer->duration;
                timer->elapsed = TRUE;
                if (timer->callback != NULL)
                {
                    (*timer->callback)();
                }
            }
        }
    }
}