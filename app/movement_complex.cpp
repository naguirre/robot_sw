// #include <app/mouvement_complexe.h>
// #include <stdio.h>

// /*************************************/
// /* SINGLETON                         */
// /*************************************/

// MOUVEMENT_COMPLEXE mouvementComplexe;
// static MOUVEMENT_COMPLEXE * this = &mouvementComplexe;


// /*************************************/
// /* STRUCTURE MACHINE A ETAT          */
// /*************************************/

// //PROTOTYPES DES FONCTIONS D'ETATS
// void MAE_MOUVEMENT_COMPLEXE_RotationInitiale(void * p);
// void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours(void * p);
// void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte(void * p);
// void MAE_MOUVEMENT_COMPLEXE_Blocage(void * p);
// void MAE_MOUVEMENT_COMPLEXE_Stop(void * p);
// void MAE_MOUVEMENT_COMPLEXE_Dodo(void * p);
// void MAE_MOUVEMENT_COMPLEXE_Termine(void * p);

// //GENERATION DE LA STRUCT MAE
// #define MAE_NOM          MAE_MOUVEMENT_COMPLEXE
// #define MAE_LISTE_ETATS  ETAT(MAE_MOUVEMENT_COMPLEXE_RotationInitiale) \
//                          ETAT(MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours) \
//                          ETAT(MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte) \
//                          ETAT(MAE_MOUVEMENT_COMPLEXE_Blocage) \
//                          ETAT(MAE_MOUVEMENT_COMPLEXE_Dodo) \
//                          ETAT(MAE_MOUVEMENT_COMPLEXE_Termine) \
//                          DERNIER_ETAT(MAE_MOUVEMENT_COMPLEXE_Stop)
// #include <mae/createmae.h>


// void MOUVEMENT_COMPLEXE_Init(ASSERVISSEMENT * longitudinal, ASSERVISSEMENT * angulaire)
// {
//     this->membres.asservissementAngulaire = angulaire;
//     this->membres.asservissementLongitudinal = longitudinal;
//     MAE_Init(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Dodo, 0, 0, ETAT_MAE_MOUVEMENT_COMPLEXE_Dodo,
//              MAE_MOUVEMENT_COMPLEXE_NB_ETATS, MAE_MOUVEMENT_COMPLEXE_Fonctions, this);
    
    
//     this->etat.etatMouvement = MOVEMENT_STOP;
//     this->etat.nbBlocages = 0;
// }

// void MOUVEMENT_COMPLEXE_ActualiserConsigne()
// {
//     float consigneLongitudinale, consigneAngulaire, angle, deltaAngle, targetX, targetY;
    
//     switch (this->consigne.type)
//     {
//         case MOVEMENT_GOTO:
//         case MOVEMENT_SPLITTED_GOTO:
//             if(this->consigne.parametres.goTo.speed > 0.0)
//             {
//                 consigneLongitudinale = ODOMETRIE_GetPositionLongitudinale() + Maths::Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y);
//                 consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + Maths::DeltaHeading(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y);

//             }else{
//                 consigneLongitudinale = ODOMETRIE_GetPositionLongitudinale() - Maths::Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y);
//                 consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + Maths::DeltaHeadingReverse(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y);
//             }
//             ASSERVISSEMENT_SetMaxSpeed(this->membres.asservissementLongitudinal, this->consigne.parametres.goTo.speed);
//             ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementLongitudinal, POSITION_CONTROLLER, consigneLongitudinale);
//             ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementAngulaire, POSITION_CONTROLLER, consigneAngulaire);
//             break;
            
//         case MOVEMENT_GOTO_THROUGH:
//             consigneLongitudinale = this->consigne.parametres.GotoThrough.speed;
//             consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + Maths::DeltaHeading(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y);
//             ASSERVISSEMENT_SetMaxSpeed(this->membres.asservissementLongitudinal, this->consigne.parametres.splittedGoTo.speed);
//             ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementLongitudinal, SPEED_CONTROLLER, consigneLongitudinale);
//             ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementAngulaire, POSITION_CONTROLLER, consigneAngulaire);
//             break;
            
//         case MOVEMENT_ROTATION_AROUND_POINT:
//             consigneLongitudinale = this->consigne.parametres.rotationAroundPoint.speed;
//             deltaAngle = (ODOMETRIE_GetVitesseLongitudinale())/(2.0*M_PI*this->consigne.parametres.rotationAroundPoint.radius)*0.05;
//             angle = atan2f(ODOMETRIE_GetY() - this->consigne.parametres.rotationAroundPoint.y, ODOMETRIE_GetX() - this->consigne.parametres.rotationAroundPoint.x);
//             if(this->consigne.parametres.rotationAroundPoint.sens == DIRECTION_COUNTERCLOCKWISE)
//             {
//                 targetX = this->consigne.parametres.rotationAroundPoint.x + this->consigne.parametres.rotationAroundPoint.radius*cosf(angle + deltaAngle);
//                 targetY = this->consigne.parametres.rotationAroundPoint.y + this->consigne.parametres.rotationAroundPoint.radius*sinf(angle + deltaAngle);
//             }
//             else{
//                 targetX = this->consigne.parametres.rotationAroundPoint.x + this->consigne.parametres.rotationAroundPoint.radius*cosf(angle - deltaAngle);
//                 targetY = this->consigne.parametres.rotationAroundPoint.y + this->consigne.parametres.rotationAroundPoint.radius*sinf(angle - deltaAngle);
//             }

//             consigneAngulaire = ODOMETRIE_GetPositionAngulaire() + Maths::DeltaHeading(ODOMETRIE_GetTheta(), ODOMETRIE_GetX(), ODOMETRIE_GetY(),
//                                                                                   targetX, targetY);
            
//             ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementLongitudinal, SPEED_CONTROLLER, consigneLongitudinale);
//             ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementAngulaire, POSITION_CONTROLLER, consigneAngulaire);
//             break;
            
//         default:
//             break;
//     }
// }

// void MOUVEMENT_COMPLEXE_CalculerRotationInitiale()
// {
//     switch (this->consigne.type)
//     {
//         case MOVEMENT_GOTO:
//             if(this->consigne.parametres.goTo.speed > 0.0)
//                 MOUVEMENT_ANGULAIRE_PointerVers(this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y);
//             else
//                 MOUVEMENT_ANGULAIRE_PointerDos(this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y);
//             break;
            
//         case MOVEMENT_SPLITTED_GOTO:
//             MOUVEMENT_ANGULAIRE_PointerVers(this->consigne.parametres.splittedGoTo.x, this->consigne.parametres.splittedGoTo.y);
//             break;
        
//         case MOVEMENT_GOTO_THROUGH:
//             MOUVEMENT_ANGULAIRE_PointerVers(this->consigne.parametres.GotoThrough.x, this->consigne.parametres.GotoThrough.y);
//             break;
            
//         case MOVEMENT_ROTATION_AROUND_POINT:
//             MOUVEMENT_ANGULAIRE_TangenterCercle(this->consigne.parametres.rotationAroundPoint.x, this->consigne.parametres.rotationAroundPoint.y,
//                                                 this->consigne.parametres.rotationAroundPoint.sens);
//             break;
            
//         default:
//             break;
//     }
// }

// bool MOUVEMENT_COMPLEXE_CibleEstDevant(float x, float y)
// {
//     float currentX = ODOMETRIE_GetX();
//     float currentY = ODOMETRIE_GetY();
//     float currentAngle = ODOMETRIE_GetTheta();
    
//     return (bool)(Maths::Abs(Maths::DeltaHeading(currentAngle, currentX, currentY, x, y)) < M_PI_2);
// }

// void MOUVEMENT_COMPLEXE_AllerA(float x, float y, float speed)
// {
//     this->consigne.type = MOVEMENT_GOTO;
//     this->consigne.parametres.goTo.x = x;
//     this->consigne.parametres.goTo.y = y;
//     this->consigne.parametres.goTo.speed = speed;
//     this->etat.etatMouvement = MOVEMENT_ONGOING;
    
//     if (!MOUVEMENT_COMPLEXE_CibleEstDevant(x, y))
//     {
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
//     }else{
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
//     }
// }

// void MOUVEMENT_COMPLEXE_splittedGoTo(float x, float y, float speed)
// {
//     this->consigne.type = MOVEMENT_SPLITTED_GOTO;
//     this->consigne.parametres.splittedGoTo.x = x;
//     this->consigne.parametres.splittedGoTo.y = y;
//     this->consigne.parametres.splittedGoTo.speed = speed;
//     this->etat.etatMouvement = MOVEMENT_ONGOING;
    
//     MOUVEMENT_COMPLEXE_CalculerRotationInitiale();
//     MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
    
// }

// void MOUVEMENT_COMPLEXE_PasserPar(float x, float y, float speed)
// {
//     this->consigne.type = MOVEMENT_GOTO_THROUGH;
//     this->consigne.parametres.GotoThrough.x = x;
//     this->consigne.parametres.GotoThrough.y = y;
//     this->consigne.parametres.GotoThrough.speed = speed;
//     this->etat.etatMouvement = MOVEMENT_ONGOING;
    
//     if (!MOUVEMENT_COMPLEXE_CibleEstDevant(x, y))
//     {
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
//     }else{
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
//     }
// }

// void MOUVEMENT_COMPLEXE_TournerAutourDe(float x, float y, RotationDirection sens, float speedLongitudinale)
// {
//     this->consigne.type = MOVEMENT_ROTATION_AROUND_POINT;
//     this->consigne.parametres.rotationAroundPoint.x = x;
//     this->consigne.parametres.rotationAroundPoint.y = y;
//     this->consigne.parametres.rotationAroundPoint.sens = sens;
//     this->consigne.parametres.rotationAroundPoint.speed = speedLongitudinale;
//     this->consigne.parametres.rotationAroundPoint.radius = Maths::Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), x, y);
//     this->etat.etatMouvement = MOVEMENT_ONGOING;
    
//     MOUVEMENT_COMPLEXE_CalculerRotationInitiale();
//     MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_RotationInitiale, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
// }

// bool MOUVEMENT_COMPLEXE_VerifierBlocage()
// {
//     if (Maths::InsideBoundary(ODOMETRIE_GetVitesseLongitudinale(), LONGITUDINAL_MOVEMENT_BLOCKING_THRESHOLD))
//     {
//         if (++this->etat.nbBlocages > LONGITUDINAL_MOVEMENT_BLOCKING_NUM)
//         {
//             return true;
//         }
//     }
//     return false;
// }

// bool MOUVEMENT_COMPLEXE_VerifierPrecisionAtteinte()
// {
//     switch (this->consigne.type)
//     {
//         case MOVEMENT_GOTO_THROUGH:
//             if (Maths::Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.GotoThrough.x, this->consigne.parametres.GotoThrough.y) < 0.1)
//             {
//                 return true;
//             }else{
//                 return false;
//             }
//             break;
            
//         case MOVEMENT_GOTO:
//             if (Maths::Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.goTo.x, this->consigne.parametres.goTo.y) < 0.05)
//             {
//                 return true;
//             }else{
//                 return false;
//             }
//             break;
            
//         case MOVEMENT_SPLITTED_GOTO:
//             if (Maths::Distance(ODOMETRIE_GetX(), ODOMETRIE_GetY(), this->consigne.parametres.splittedGoTo.x, this->consigne.parametres.splittedGoTo.y) < 0.01)
//             {
//                 return true;
//             }else{
//                 return false;
//             }
//             break;
            
//         default:
//             return false;
//             break;
//     }
// }

// bool MOUVEMENT_COMPLEXE_VerifierTermine()
// {
//     if ((Maths::InsideBoundary(ODOMETRIE_GetAngularSpeed(), ANGULAR_MOVEMENT_DONE_THRESHOLD)) &&
//         (Maths::InsideBoundary(ODOMETRIE_GetVitesseLongitudinale(), LONGITUDINAL_MOVEMENT_DONE_THRESHOLD)) &&
//         (MAE_GetEtatActuel(&this->membres.mae) == ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte))
//     {
//         return true;
//     }else{
//         return false;
//     }
// }


// void MAE_MOUVEMENT_COMPLEXE_RotationInitiale(void * p)
// {
//     if (MAE_PremierPassage(&this->membres.mae))
//     {
//         MOUVEMENT_COMPLEXE_CalculerRotationInitiale();
//         fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Rotation initiale\n");
//     }
    
//     MOUVEMENT_ANGULAIRE_Run();
    
//     if ((MOUVEMENT_ANGULAIRE_GetEtatDeplacement() == MOVEMENT_DONE) ||
//         (MOUVEMENT_ANGULAIRE_GetEtatDeplacement() == MOVEMENT_ALMOST_DONE))
//     {
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours, 0, MAE_MOUVEMENT_COMPLEXE_TIMEOUT);
//     }
//     else if (MOUVEMENT_ANGULAIRE_GetEtatDeplacement() == MOVEMENT_BLOCKED)
//     {
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Blocage, 0, 0);
//     }
// }

// void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoireEnCours(void * p)
// {
//     if (MAE_PremierPassage(&this->membres.mae))
//     {
//         fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Suivi Trajetoire en cours\n");
//         this->etat.nbBlocages = 0;
//         this->etat.etatMouvement = MOVEMENT_ONGOING;
//     }
    
//     MOUVEMENT_COMPLEXE_ActualiserConsigne();
    
//     if(MOUVEMENT_COMPLEXE_VerifierPrecisionAtteinte())
//     {
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte, 0, 0);
//     }
//     else if(MOUVEMENT_COMPLEXE_VerifierBlocage())
//     {
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Blocage, 0, 0);
//     }
    
// }

// void MAE_MOUVEMENT_COMPLEXE_SuiviTrajectoirePrecisionAtteinte(void * p)
// {
//     if (MAE_PremierPassage(&this->membres.mae))
//     {
//         fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Suivi Trajetoire Precision Atteinte\n");
//         this->etat.etatMouvement = MOVEMENT_ALMOST_DONE;
//     }
//     MOUVEMENT_COMPLEXE_ActualiserConsigne();
//     //On arrete de corriger l'angle
//     //ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementAngulaire, POSITION_CONTROLLER, ODOMETRIE_GetPositionAngulaire());
    
//     if(MOUVEMENT_COMPLEXE_VerifierTermine())
//     {
//         MAE_ChangerEtat(&this->membres.mae, ETAT_MAE_MOUVEMENT_COMPLEXE_Termine, 0, 0);
//     }
    
// }

// void MAE_MOUVEMENT_COMPLEXE_Blocage(void * p)
// {
//     if (MAE_PremierPassage(&this->membres.mae))
//     {
//         this->etat.etatMouvement = MOVEMENT_BLOCKED;
//         fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Blocage\n");
//         ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementLongitudinal, SPEED_CONTROLLER, 0.0);
//         ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementAngulaire, SPEED_CONTROLLER, 0.0);
//     }
// }

// void MAE_MOUVEMENT_COMPLEXE_Termine(void * p)
// {
//     if (MAE_PremierPassage(&this->membres.mae))
//     {
//         this->etat.etatMouvement = MOVEMENT_DONE;
//         fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Termine\n");
//         ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementLongitudinal, POSITION_CONTROLLER, ODOMETRIE_GetPositionLongitudinale());
//         ASSERVISSEMENT_DefineSetPoint(this->membres.asservissementAngulaire, POSITION_CONTROLLER, ODOMETRIE_GetPositionAngulaire());
//     }
// }

// void MAE_MOUVEMENT_COMPLEXE_Stop(void * p)
// {
//     if (MAE_PremierPassage(&this->membres.mae))
//     {
//         this->etat.etatMouvement = MOVEMENT_STOP;
//         fprintf(stderr, "DEPLACEMENT | MOUVEMENT COMPLEXE | Stop\n");
//     }
// }

// void MAE_MOUVEMENT_COMPLEXE_Dodo(void * p)
// {
//     return;
// }


// MovementState MOUVEMENT_COMPLEXE_GetEtatDeplacement()
// {
//     return this->etat.etatMouvement;
// }

// void MOUVEMENT_COMPLEXE_Run()
// {
//     MAE_Automate(&this->membres.mae);
// }