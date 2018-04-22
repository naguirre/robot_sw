#include <assert.h>
#include <state_machine/StateMachine.h>
#include <tick/tick.h>
 
StateMachine::StateMachine(int state_init, int default_state, int nb_states) :
    current_state(state_init),
    last_state(state_init),
    timer(Tick::GetMillis()),
    start_wait_duration(0),
    timeout(0),
    default_state(default_state),
    nb_states(nb_states)
{
}    
 
void StateMachine::Engine(void)
{
    // tempo de travail si besoin pour l automate
    if (Tick::GetMillis() - timer > this->start_wait_duration)
    {
        this->waiting_start = false;
    }

    if (Tick::GetMillis() - timer > timeout)
    {
        this->timeout_occured = true;
    }

    // appel de la fonction correspondante a l etat courant
    if (this->current_state < this->nb_states)
    {
        const StateStruct * pStateMap = GetStateMap();
        (this->*pStateMap[this->current_state].pStateFunc)();
    }
    else
    {
        this->current_state = this->default_state; //au cas ou bug
    }
}

void StateMachine::ChangeState(int new_state, int start_wait_duration, int timeout)
{
    if(new_state < nb_states)                             // on verifie value correcte pour le nouvel etat
    {
        this->last_state          = this->current_state;  // on sauvegarde l'etat courant
        this->current_state       = new_state;            // on renseigne le nouvel etat
        this->first_time          = true;                 // on positionne le booleen pour signifier le 1er acces a l etat
        this->timer               = Tick::GetMillis();
        this->start_wait_duration = start_wait_duration;
        this->timeout             = timeout;
        this->waiting_start       = true;
        this->timeout_occured     = false;
    }
    else
    {
        this->current_state = this->default_state; //au cas ou value en dehors des etats connus
    }
}

void StateMachine::LastState(void)
{
    uint8_t tmp;

    //permutation entre last_state et current_state
    tmp                 = this->last_state;
    this->last_state    = this->current_state;
    this->current_state = tmp;
    this->first_time    = true;    //on positionne le booleen pour signifier le 1er acces a l etat
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
