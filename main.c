#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <types.h>
#include <timer/timer.h>

#include <app/mouvement.h>

TIMER ordonnanceur;

#define PERIODE_ORDONNANCEUR        (0.01)

void run()
{
    MOUVEMENT_Run();
}

void init()
{
    MOUVEMENT_Init(PERIODE_ORDONNANCEUR);
    
    //On lance l'ordonnanceur
    TIMER_Init(&ordonnanceur, PERIODE_ORDONNANCEUR, &run);
}

int main(int argc, char **argv)
{
    init();

    MOUVEMENT_Avancer(100.0);
    
    while (TRUE)
    {
        usleep(10);
    }
    
    return 0;
}

