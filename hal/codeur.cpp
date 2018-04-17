#include <hal/codeur.h>
#include <maths/maths.h>

void CODEUR::CalculerFacteurEchelle()
{
    parametres.facteurEchelle = 2.0 * PI / (4.0 * (float) (parametres.nombrePas) * parametres.reducteur);
}

CODEUR::CODEUR(uint8_t id, uint16_t nombrePasCodeur, float reducteur, float periode)
{
    etat.angleBrut = 0.0;
    etat.angleReel = 0.0;

    parametres.nombrePas = nombrePasCodeur;
    parametres.reducteur = reducteur;
    parametres.periode   = periode;

    qei = QEI_Init(QEI_CHANNELS[id]);

    //Calcul de la distance représentée par un pas
    CalculerFacteurEchelle();
}

void CODEUR::ActualiserEtat()
{
    float angleBrutPrecedent = etat.angleBrut;

    etat.angleBrut = (float)qei->GetNombrePas()*parametres.facteurEchelle;
    etat.angleReel = MATHS_mod2pi(etat.angleBrut);
    etat.vitesseAngulaire = (etat.angleBrut - angleBrutPrecedent) / parametres.periode;
}

float CODEUR::GetAngleBrut()
{
    return etat.angleBrut;
}

void CODEUR::SetAngleBrut(float Val)
{
    etat.angleBrut = Val;
    etat.vitesseAngulaire = 0.0;
}

float CODEUR::GetAngleReel()
{
    return etat.angleReel;
}

float CODEUR::GetVitesseAngulaire()
{
    return etat.vitesseAngulaire;
}
