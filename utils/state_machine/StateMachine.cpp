#include <assert.h>
#include <state_machine/StateMachine.h>
#include <tick/tick.h>
 
StateMachine::StateMachine(int state_init, int default_state, int nb_states) :
    current_state(state_init),
    last_state(state_init),
    timer(Tick::GetMillis()),
    nb_states(nb_states),
    timeout(0),
    start_wait_duration(0),
    default_state(default_state)
{
}    
 
void StateMachine::Engine(void)
{ 
    if (Tick::GetMillis() - timer > this->start_wait_duration)
    {
        this->waiting_start = false;
    }

    if (Tick::GetMillis() - timer > timeout)
    {
        this->timeout_occured = true;
    }

    if (this->current_state < this->nb_states)
    {
        // Call the current state method
        const StateStruct * pStateMap = GetStateMap();
        (this->*pStateMap[this->current_state].pStateFunc)();
    }
    else
    {
        // Fallback
        this->current_state = this->default_state; 
    }
}

void StateMachine::ChangeState(int new_state, int start_wait_duration, int timeout)
{
    if(new_state < nb_states)                             // check the new state value integrity
    {
        this->last_state          = this->current_state;  // store the current state
        this->current_state       = new_state;            // update the new state
        this->first_time          = true;                 // set the bool to indicate first time in this state
        this->timer               = Tick::GetMillis();
        this->start_wait_duration = start_wait_duration;
        this->timeout             = timeout;
        this->waiting_start       = true;
        this->timeout_occured     = false;
    }
    else
    {
        this->current_state = this->default_state; // fallback
    }
}

void StateMachine::LastState(void)
{
    uint8_t tmp;

    // switch last_state and current_state
    tmp                 = this->last_state;
    this->last_state    = this->current_state;
    this->current_state = tmp;

    // Set the bool to indicate first time in this state
    this->first_time    = true;
}

bool StateMachine::WaitingStartDone(void)
{
    return !this->waiting_start;
}

bool StateMachine::CheckTimeout(void)
{
    return this->timeout_occured;
}

bool StateMachine::FirstTime(void)
{
    if (this->first_time)
    {
        this->first_time = false;
        return true;
    }
    else
    {
        return false;
    }
}

int StateMachine::GetCurrentState(void)
{
    return this->current_state;
}

int StateMachine::GetLastState(void)
{
    return last_state;
}
