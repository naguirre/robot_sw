#include <maths/maths.h>


float MATHS_mod2pi(float angleBrut)
{
    while(angleBrut > PI){
        angleBrut -= 2*PI;
    }
    while(angleBrut <= -PI){
        angleBrut += 2*PI;
    }
    return angleBrut;
}

float MATHS_abs(float valeur)
{
    if(valeur < 0.0)
        return -valeur;
    else
        return valeur;
}

BOOL  MATHS_estPositif(float valeur)
{
    return (valeur >= 0.0);
}

float MATHS_Saturer(float val, float borne)
{
    if (val > borne)
		return borne;
	else if (val < -borne)
		return -borne;
	else
		return val;
}

float MATHS_Borner(float val, float borneMin, float borneMax)
{
    if (val > borneMax)
		return borneMax;
	else if (val < -borneMin)
		return -borneMin;
	else
		return val;
}

uint16_t MATHS_BornerInt16(uint16_t val, uint16_t borneMin, uint16_t borneMax)
{
    if (val > borneMax)
		return borneMax;
	else if (val < -borneMin)
		return -borneMin;
	else
		return val;
}

float MATHS_SaturerDerivee(float val, float valPrecedente, float periode, float borne)
{
    if (val - valPrecedente > borne * periode)
		return valPrecedente + borne * periode;
	else if (val - valPrecedente < -borne * periode)
		return valPrecedente - borne * periode;
    else
        return val;
}

BOOL MATHS_EstDansBorne(float val, float borne)
{
    if ((val >= -borne) && (val <= borne))
		return TRUE;
	else
		return FALSE;
}

BOOL MATHS_EstDansBorneInt16(int16_t val, int16_t borne)
{
    if ((val >= -borne) && (val <= borne))
		return TRUE;
	else
		return FALSE;
}

float MATHS_DeltaCap(float thetaActuel, float xActuel, float yActuel, float xVise, float yVise)
{
    float deltaX = xVise - xActuel;
    float deltaY = yVise - yActuel;
    float angleVoulu = atan2f(deltaY, deltaX);
    return MATHS_mod2pi(angleVoulu - thetaActuel);
}

float MATHS_DeltaCapContraire(float thetaActuel, float xActuel, float yActuel, float xVise, float yVise)
{
    float deltaX = xVise - xActuel;
    float deltaY = yVise - yActuel;
    float angleVoulu = atan2f(deltaY, deltaX);
    return MATHS_mod2pi(angleVoulu + M_PI - thetaActuel);
}

float MATHS_Distance(float x1, float y1, float x2, float y2)
{
    return sqrtf((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int16_t MATHS_DistanceInt16(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    return (int16_t)sqrtf((float)((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}