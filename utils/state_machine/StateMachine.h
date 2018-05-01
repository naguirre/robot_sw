#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdlib.h>
#include <types.h>

class StateMachine;

typedef void (StateMachine::*StateFunc)(void);
 
// base class for state machines
class StateMachine 
{

public:

    StateMachine(unsigned int stateInit, unsigned int defaultState, unsigned int nbStates);
    virtual ~StateMachine() {}

protected:

    void Engine(void);
    void ChangeState(unsigned int newState, int startWaitDuration, int timeout);
    void LastState(void);
    bool WaitingStartDone(void);
    bool CheckTimeout(void);
    bool FirstTime(void);
    unsigned int GetCurrentState(void);
    unsigned int GetLastState(void);
    virtual const StateFunc* GetStateMap() = 0;

private:

    const unsigned int  nbStates;
    unsigned int        currentState;
    unsigned int        lastState;
    int                 timer;                // tick when arrived in current state
    uint32_t            timeout;
    bool                firstTime;            // first time in current state
    bool                timeoutOccured;       // in timeout ?
    uint32_t            startWaitDuration;    // optionnal wait before executing state action
    bool                waitingStart;
    unsigned int        defaultState;         // case of bad current state value
};
 
#define BEGIN_STATE_MAP \
const StateFunc* GetStateMap() {\
    static const StateFunc StateMap[] = { 
 
#define STATE_MAP_ENTRY(entry)\
    reinterpret_cast<StateFunc>(&entry),
 
#define END_STATE_MAP \
    NULL\
    }; \
    return &StateMap[0]; }
 
#endif