#include <assert.h>
#include <state_machine/StateMachine.h>
#include <tick/tick.h>
 
StateMachine::StateMachine(unsigned int stateInit, unsigned int defaultState, unsigned int nbStates) :
    nbStates(nbStates),
    currentState(stateInit),
    lastState(stateInit),
    timer(Tick::GetMillis()),
    timeout(0),
    startWaitDuration(0),
    waitingStart(0),
    defaultState(defaultState)
{
}    

void StateMachine::Engine(void)
{ 
    if (Tick::GetMillis() - timer > this->startWaitDuration)
    {
        this->waitingStart = false;
    }

    if (Tick::GetMillis() - timer > timeout)
    {
        this->timeoutOccured = true;
    }

    if (this->currentState < this->nbStates)
    {
        // Call the current state method
        const StateFunc * pStateMap = GetStateMap();
        (this->*pStateMap[this->currentState])();
    }
    else
    {
        // Fallback
        this->currentState = this->defaultState; 
    }
}

void StateMachine::ChangeState(unsigned int newState, int startWaitDuration, int timeout)
{
    if(newState < nbStates)                              // check the new state value integrity
    {
        this->lastState          = this->currentState;   // store the current state
        this->currentState       = newState;             // update the new state
        this->firstTime          = true;                 // set the bool to indicate first time in this state
        this->timer              = Tick::GetMillis();
        this->startWaitDuration  = startWaitDuration;
        this->timeout            = timeout;
        this->waitingStart       = true;
        this->timeoutOccured     = false;
    }
    else
    {
        this->currentState = this->defaultState; // fallback
    }
}

void StateMachine::LastState(void)
{
    uint8_t tmp;

    // switch lastState and currentState
    tmp                = this->lastState;
    this->lastState    = this->currentState;
    this->currentState = tmp;

    // Set the bool to indicate first time in this state
    this->firstTime    = true;
}

bool StateMachine::WaitingStartDone(void)
{
    return !this->waitingStart;
}

bool StateMachine::CheckTimeout(void)
{
    return this->timeoutOccured;
}

bool StateMachine::FirstTime(void)
{
    if (this->firstTime)
    {
        this->firstTime = false;
        return true;
    }
    else
    {
        return false;
    }
}

unsigned int StateMachine::GetCurrentState(void)
{
    return this->currentState;
}

unsigned int StateMachine::GetLastState(void)
{
    return lastState;
}
