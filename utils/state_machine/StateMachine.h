#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdlib.h>
 
struct StateStruct;
 
// base class for state machines
class StateMachine 
{
public:
    StateMachine(int state_init, int default_state, int nb_states);
    virtual ~StateMachine() {}
protected:
    void Engine(void);
    void ChangeState(int new_state, int start_wait_duration, int timeout);
    void LastState(void);
    bool WaitingStartDone(void);
    bool CheckTimeout(void);
    bool FirstTime(void);
    int  GetCurrentState(void);
    int  GetLastState(void);
    virtual const StateStruct* GetStateMap() = 0;
private:
    const int  nb_states;
    int  current_state;
    int  last_state;
    int  timer;                // horloge à l'arrivée dans l'état actuel
    int  timeout;
    bool first_time;           // premier passage dans l'état actuel
    bool timeout_occured;      // en timeout ?
    int  start_wait_duration;  // pour attente avant de commencer action
    bool waiting_start;
    int  default_state;        // en cas d'etat incoherent
};
 
typedef void (StateMachine::*StateFunc)(void);
struct StateStruct 
{
    StateFunc pStateFunc;
};
 
#define BEGIN_STATE_MAP \
public:\
const StateStruct* GetStateMap() {\
    static const StateStruct StateMap[] = { 
 
#define STATE_MAP_ENTRY(entry)\
    { reinterpret_cast<StateFunc>(&entry) },
 
#define END_STATE_MAP \
    { NULL }\
    }; \
    return &StateMap[0]; }
 
#endif