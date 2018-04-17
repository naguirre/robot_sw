#ifndef DEPLACEMENT_TYPES_H
#define DEPLACEMENT_TYPES_H

/*************************************/
/* TYPES                             */
/*************************************/

typedef enum
{
    DEPLACEMENT_SENS_ARRIERE,
    DEPLACEMENT_SENS_AVANT,
}DEPLACEMENT_SENS_TRANSLATION;

typedef enum
{
    DEPLACEMENT_SENS_HORAIRE,
    DEPLACEMENT_SENS_TRIGO,
}DEPLACEMENT_SENS_ROTATION;

typedef enum
{
    DEPLACEMENT_MOUVEMENT_EN_COURS,
    DEPLACEMENT_MOUVEMENT_BLOQUE,
    DEPLACEMENT_MOUVEMENT_PRECISION_ATTEINTE,
    DEPLACEMENT_MOUVEMENT_TERMINE,
    DEPLACEMENT_MOUVEMENT_STOP,
}DEPLACEMENT_MOUVEMENT_ETAT;

typedef enum
{
    DEPLACEMENT_CONSIGNE_STOP,
    DEPLACEMENT_CONSIGNE_AVANCER,
    DEPLACEMENT_CONSIGNE_TOURNER,
    DEPLACEMENT_CONSIGNE_POINTER_VERS,
    DEPLACEMENT_CONSIGNE_TANGENTER_CERCLE,
    DEPLACEMENT_CONSIGNE_TRANSLATION_INFINIE,
    DEPLACEMENT_CONSIGNE_ROTATION_INFINIE,
    DEPLACEMENT_CONSIGNE_ALLER_A,
    DEPLACEMENT_CONSIGNE_ALLER_A_DECOMPOSE,
    DEPLACEMENT_CONSIGNE_TOURNER_AUTOUR_DE,
    DEPLACEMENT_CONSIGNE_PASSER_PAR,
}DEPLACEMENT_CONSIGNE_TYPE;

typedef union
{
    struct __attribute__((packed, aligned(1))){
        float distance;
    }avancer;
    struct __attribute__((packed, aligned(1))){
        float angle;
    }tourner;
    struct __attribute__((packed, aligned(1))){
        float x;
        float y;
    }pointerVers;
    struct __attribute__((packed, aligned(1))){
        float x;
        float y;
        float vitesse;
    }allerA;
    struct __attribute__((packed, aligned(1))){
        float vitesse;
    }translationInfinie;
    struct __attribute__((packed, aligned(1))){
        float vitesse;
    }rotationInfinie;
    struct __attribute__((packed, aligned(1))){
        float x;
        float y;
        DEPLACEMENT_SENS_ROTATION sens;
        float vitesse;
        float rayon;
    }tournerAutourDe;
    struct __attribute__((packed, aligned(1))){
        float x;
        float y;
        float vitesse;
    }AllerADecompose;
    struct __attribute__((packed, aligned(1))){
        float x;
        float y;
        float vitesse;
    }passerPar;
    struct __attribute__((packed, aligned(1))){
        float x;
        float y;
        DEPLACEMENT_SENS_ROTATION sens;
    }tangenterCercle;
}DEPLACEMENT_CONSIGNE_PARAMETRES;

typedef enum
{
    DEPLACEMENT_TRIGGER_DISTANCE_PARCOURUE,
    DEPLACEMENT_TRIGGER_VITESSE_ATTEINTE,
    DEPLACEMENT_TRIGGER_PROXIMITE_POINT,
    DEPLACEMENT_TRIGGER_
}DEPLACEMENT_TRIGGER_TYPE;

typedef union
{
    struct __attribute__((packed, aligned(1))){
        float distance;
    }distanceParcourue;
    struct __attribute__((packed, aligned(1))){
        float vitesse;
    }vitesseAtteinte;
}DEPLACEMENT_TRIGGER_PARAMETRES;

#endif
