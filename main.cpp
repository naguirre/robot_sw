#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <types.h>
#include <drivers/timer.h>

#include <app/robot.h>

#include <logs/logs.h>

#define PERIODE_ORDONNANCEUR        (0.01)

typedef enum
{
    COMMAND_NONE = 0,
    COMMAND_LEFT,
    COMMAND_RIGHT,
    COMMAND_FORWARD,
    COMMAND_BACKWARD
}RobotCommand;

#define ROBOT_COMMAND_NUM           10
#define ROBOT_COMMAND_DISTANCE      0.2

RobotCommand commands[ROBOT_COMMAND_NUM] = {
    COMMAND_FORWARD,
    COMMAND_LEFT,
    COMMAND_FORWARD,
    COMMAND_RIGHT,
    COMMAND_RIGHT,
    COMMAND_BACKWARD,
    COMMAND_LEFT,
    COMMAND_FORWARD,
    COMMAND_FORWARD,
    COMMAND_RIGHT
};

int main(int argc, char **argv)
{
    int command_index = 0;

    Robot * robot = new Robot(PERIODE_ORDONNANCEUR, true);
 
    while ((command_index < ROBOT_COMMAND_NUM) && (commands[command_index] != COMMAND_NONE))
    {
        switch (commands[command_index])
        {
            case COMMAND_LEFT:
                robot->Rotate(PI_2);
                break;

            case COMMAND_RIGHT:
                robot->Rotate(-PI_2);
                break;

            case COMMAND_FORWARD:
                robot->Translate(ROBOT_COMMAND_DISTANCE);
                break;

            case COMMAND_BACKWARD:
                robot->Translate(-ROBOT_COMMAND_DISTANCE);
                break;

            default:
                exit(1);
                break;
        }
        
        while (robot->CheckMovementDone() == false)
        {
            usleep(10000);
            robot->Run();
        }

        command_index++;
    }
    
    return 0;
}
