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
    int  timer;                // tick when arrived in current state
    int  timeout;
    bool first_time;           // first time in current state
    bool timeout_occured;      // in timeout ?
    int  start_wait_duration;  // optionnal wait before executing state action
    bool waiting_start;
    int  default_state;        // case of bad current state value
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