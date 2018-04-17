#include <app/pid.h>

void PID_CalculerPidDiscret(PID * this)
{
    this->parametres.pidDiscret.num[0] = this->parametres.ki*this->parametres.periode/2 - this->parametres.kp;
	this->parametres.pidDiscret.num[1] = this->parametres.ki*this->parametres.periode/2 + this->parametres.kp;
	this->parametres.pidDiscret.den[0] = -1.0;
	this->parametres.pidDiscret.den[1] = 1.0;
}

void PID_Init(PID * this, float periode, float kp, float ki, float consigne, float commandeMax)
{
    this->parametres.periode = periode;
    this->parametres.commandeMax = commandeMax;
    this->parametres.kp = kp;
    this->parametres.ki = ki;
    PID_CalculerPidDiscret(this);
    
    this->entrees.consigne = consigne;
}

void PID_CalculerCommande(PID * this)
{
	//Calcul de l'erreur entre l'Žtat et la consigne
	this->etat.erreur = this->entrees.consigne - this->entrees.mesure;

    this->etat.commande = (float)( this->parametres.pidDiscret.num[1]*this->etat.erreur
                                 + this->parametres.pidDiscret.num[0]*this->etat.erreurPrecedente
                                 - this->parametres.pidDiscret.den[0]*this->etat.commandePrecedente)
                                 / this->parametres.pidDiscret.den[1];
    
    this->etat.commandePrecedente = this->etat.commande;
    this->etat.commande = MATHS_Saturer(this->etat.commande, this->parametres.commandeMax);
    this->sortie.commandeNormalisee = this->etat.commande / this->parametres.commandeMax;
    
    this->etat.erreurPrecedente = this->etat.erreur;
}

float PID_Run(PID * this, float consigne, float mesure)
{
    this->entrees.consigne = consigne;
    this->entrees.mesure   = mesure;
    
    PID_CalculerCommande(this);
    
    return this->sortie.commandeNormalisee;
}

void PID_SetKp(PID * this, float kp)
{
    this->parametres.kp = kp;
    PID_CalculerPidDiscret(this);
}

void PID_SetKi(PID * this, float ki)
{
    this->parametres.ki = ki;
    PID_CalculerPidDiscret(this);
}

float PID_GetKp(PID * this)
{
    return this->parametres.kp;
}

float PID_GetKi(PID * this)
{
    return this->parametres.ki;
}

float PID_GetConsigne(PID * this)
{
    return this->entrees.consigne;
}


