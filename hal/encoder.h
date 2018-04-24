#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <types.h>

#include <drivers/qei.h>


class Encoder
{

private:

    Qei * qei;   //Abstraction du périph QEI utilisé par le codeur
    
    struct{
        float    reduction;     // Réducteur entre l'arbre de sortie du moteur et l'axe du codeur
        uint16_t stepsCount;    // Nombre de pas par tour du codeur
        float    scaleFactor;   // Rapport entre un pas de codeur et une portion d'angle
        float    period;        // Période d'acquisition des données en secondes (pour calcul de speed)
    }settings;
    
    struct{
        float    angle;         // angle absolu de l'arbre du moteur en degrés
        float    speed;         // en degré/seconde
    }state;

    const Qei::Channel QEI_CHANNELS[2] = {Qei::CHANNEL0, Qei::CHANNEL1};

    void ComputeScaleFactor();

public:

    Encoder(uint8_t idMotor, uint16_t stepsCount, float reduction, float period);

    ~Encoder(void);

    void Update();

    float GetAngle();

    void  SetAngle(float value);

    float GetSpeed();
};

#endif /* __ENCODER_H__ */
