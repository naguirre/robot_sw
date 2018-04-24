#ifndef __MATHS_H__
#define __MATHS_H__

#include <math.h>
#include <types.h>

#define PI   3.141592653589793
#define PI_2 1.5707963267949

class Maths
{

public:

    static float mod2pi(float rawAngle);

    static float Abs(float value);

    static bool  IsPositive(float value);

    static float Limit(float val, float boundary);

    static float Limit(float val, float boundaryMin, float boundaryMax);

    static float LimitVariation(float val, float lastValue, float period, float boundary);

    static bool  InsideBoundary(float val, float boundary);

    static float DeltaHeading(float currentAngle, float currentX, float currentY, float targetX, float targetY);

    static float DeltaHeadingReverse(float currentAngle, float currentX, float currentY, float targetX, float targetY);

    static float Distance(float x1, float y1, float x2, float y2);
};

#endif /* __MATHS_H__ */
