#ifndef Robot2_0_maths_h
#define Robot2_0_maths_h

#include <math.h>
#include <types.h>

#define PI   3.141592653589793
#define PI_2 1.5707963267949

#define MATHS_SENS(a)    ((a>0)?(1):(-1))
#define MATHS_POSITIF(a) ((a>0)?(1):(0))
#define MATHS_NEGATIF(a) ((a>0)?(0):(1))

float MATHS_mod2pi(float angleBrut);

float MATHS_abs(float valeur);

BOOL  MATHS_estPositif(float valeur);

//Fait en sorte que Val soit compris dans l'interval [-Abs ; Abs]
float MATHS_Saturer(float val, float borne);

float MATHS_Borner(float val, float borneMin, float borneMax);

uint16_t MATHS_BornerInt16(uint16_t val, uint16_t borneMin, uint16_t borneMax);

float MATHS_SaturerDerivee(float val, float valPrecedente, float periode, float borne);

BOOL MATHS_EstDansBorne(float val, float borne);

BOOL MATHS_EstDansBorneInt16(int16_t val, int16_t borne);

float MATHS_DeltaCap(float thetaActuel, float xActuel, float yActuel, float xVise, float yVise);

float MATHS_DeltaCapContraire(float thetaActuel, float xActuel, float yActuel, float xVise, float yVise);

float MATHS_Distance(float x1, float y1, float x2, float y2);

int16_t MATHS_DistanceInt16(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

#endif
