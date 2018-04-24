#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <types.h>
#include <drivers/timer.h>

#include <app/robot.h>

#include <logs/logs.h>

#define PERIODE_ORDONNANCEUR        (0.01)

Robot * robot;

void run()
{
    robot->Run();
}

void init()
{
    INF("Start init");

    robot = new Robot(PERIODE_ORDONNANCEUR, true);

    INF("Init done");
}

int main(int argc, char **argv)
{
    init();

    robot->Translate(1.0);

    while (true)
    {
        usleep(10000);
        run();
    }

    return 0;
}

