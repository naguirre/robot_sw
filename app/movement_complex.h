// #ifndef MOUVEMENT_COMPLEXE_H
// #define MOUVEMENT_COMPLEXE_H

// /*************************************/
// /* INCLUDES                          */
// /*************************************/

// #include <types.h>
// #include <math.h>

// #include <maths/maths.h>
// #include <mae/mae.h>

// #include <app/asservissement.h>
// #include <app/odometrie.h>
// #include <app/mixer.h>
// #include <app/interfaces.h>

// #include <app/mouvement_angulaire.h>
// #include <app/mouvement_longitudinal.h>

// /*************************************/
// /* DEFINES                           */
// /*************************************/

// #define MAE_MOUVEMENT_COMPLEXE_TIMEOUT    5000

// /*************************************/
// /* OBJET STRUCTURE                   */
// /*************************************/

// typedef struct
// {
//     struct{
//         MovementType       type;
//         MovementSettings parametres;
//     }consigne;
//     struct{
//         MAE            mae;
//         ASSERVISSEMENT * asservissementLongitudinal;
//         ASSERVISSEMENT * asservissementAngulaire;
//     }membres;
//     struct{
//         uint8_t nbBlocages;
//         MovementState etatMouvement;
//     }etat;
// }MOUVEMENT_COMPLEXE;


// /*************************************/
// /* FONCTIONS                         */
// /*************************************/

// void MOUVEMENT_COMPLEXE_Init(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire);

// void MOUVEMENT_COMPLEXE_Run();

// void MOUVEMENT_COMPLEXE_AllerA(float x, float y, float speed);

// void MOUVEMENT_COMPLEXE_splittedGoTo(float x, float y, float speed);

// void MOUVEMENT_COMPLEXE_PasserPar(float x, float y, float speed);

// void MOUVEMENT_COMPLEXE_TournerAutourDe(float x, float y, RotationDirection sens, float speedLongitudinale);

// MovementState MOUVEMENT_COMPLEXE_GetEtatDeplacement();

// #endif
