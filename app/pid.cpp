#include <app/pid.h>

void PID::CalculerPidDiscret()
{
    parametres.pidDiscret.num[0] = parametres.ki*parametres.periode/2 - parametres.kp;
  	parametres.pidDiscret.num[1] = parametres.ki*parametres.periode/2 + parametres.kp;
  	parametres.pidDiscret.den[0] = -1.0;
  	parametres.pidDiscret.den[1] = 1.0;
}

void PID::Init(float periode, float kp, float ki, float consigne, float commandeMax)
{
    parametres.periode = periode;
    parametres.commandeMax = commandeMax;
    parametres.kp = kp;
    parametres.ki = ki;
    PID::CalculerPidDiscret(this);

    entrees.consigne = consigne;
}

void PID::CalculerCommande()
{
	//Calcul de l'erreur entre l'�tat et la consigne
	etat.erreur = entrees.consigne - entrees.mesure;

    etat.commande = (float)( parametres.pidDiscret.num[1]*etat.erreur
                                 + parametres.pidDiscret.num[0]*etat.erreurPrecedente
                                 - parametres.pidDiscret.den[0]*etat.commandePrecedente)
                                 / parametres.pidDiscret.den[1];

    etat.commandePrecedente = etat.commande;
    etat.commande = MATHS_Saturer(etat.commande, parametres.commandeMax);
    sortie.commandeNormalisee = etat.commande / parametres.commandeMax;

    etat.erreurPrecedente = etat.erreur;
}

float PID::Run(float consigne, float mesure)
{
    entrees.consigne = consigne;
    entrees.mesure   = mesure;

    CalculerCommande(this);

    return sortie.commandeNormalisee;
}

void PID::SetKp(float kp)
{
    parametres.kp = kp;
    CalculerPidDiscret(this);
}

void PID::SetKi(float ki)
{
    parametres.ki = ki;
    CalculerPidDiscret(this);
}

float PID::GetKp()
{
    return parametres.kp;
}

float PID::GetKi()
{
    return parametres.ki;
}

float PID::GetConsigne()
{
    return entrees.consigne;
}
