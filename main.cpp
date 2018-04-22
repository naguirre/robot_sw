#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <types.h>
#include <drivers/timer.h>

#include <app/robot.h>

TIMER ordonnanceur;

#define PERIODE_ORDONNANCEUR        (0.01)

Robot * robot;

void run()
{
    robot->Run();
}

void init()
{
    robot = new Robot(PERIODE_ORDONNANCEUR);

    //On lance l'ordonnanceur
    TIMER_Init(&ordonnanceur, PERIODE_ORDONNANCEUR, &run);
}

int main(int argc, char **argv)
{
    init();

    robot->Translate(100.0);

    while (true)
    {
        usleep(10);
    }
    
    return 0;
}

