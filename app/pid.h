#ifndef PID_H
#define PID_H

#include <maths/maths.h>
#include <types.h>

/*************************************/
/* OBJET STRUCTURE                   */
/*************************************/

typedef struct
{
    struct{
        float periode;      //periode du correcteur
       	float kp;
        float ki;
        struct{
            float num[2];	//Parametres du num du correcteur (a=num[i] <=> a*z^i)
            float den[2];
        }pidDiscret;
        float commandeMax;  //pour limitation et normalisation
    }parametres;

    struct{
        float consigne;         //consigne
        float mesure;           //mesure de la sortie
    }entrees;
	
    struct{
        float erreur;
        float erreurPrecedente;     //pour equations de récurrence
        float commande;
        float commandePrecedente;   //pour equation de récurrence
    }etat;
    
    struct{
        float commandeNormalisee;
    }sortie;
} PID;

/*************************************/
/* FONCTIONS                         */
/*************************************/

void PID_Init(PID * this, float periode, float kp, float ki, float consigne, float commandeMax);

float PID_Run(PID * this, float consigne, float mesure);

void PID_SetKp(PID * this, float kp);

void PID_SetKi(PID * this, float ki);

float PID_GetKp(PID * this);

float PID_GetKi(PID * this);

float PID_GetConsigne(PID * this);


#endif
