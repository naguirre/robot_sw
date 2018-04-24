#include <maths/maths.h>


float Maths::mod2pi(float rawAngle)
{
    while (rawAngle > PI)
    {
        rawAngle -= 2.0 * PI;
    }
    while (rawAngle <= -PI)
    {
        rawAngle += 2.0 * PI;
    }
    return rawAngle;
}

float Maths::Abs(float value)
{
    if (value < 0.0)
    {
        return -value;
    }
    else
    {
        return value;
    }
}

bool Maths::IsPositive(float value)
{
    return (value >= 0.0);
}

float Maths::Limit(float val, float boundary)
{
    if (val > boundary)
    {
        return boundary;
    }
    else if (val < -boundary)
    {
        return -boundary;
    }
    else
    {
        return val;
    }
}

float Maths::Limit(float val, float boundaryMin, float boundaryMax)
{
    if (val > boundaryMax)
    {
        return boundaryMax;
    }
    else if (val < -boundaryMin)
    {
        return -boundaryMin;
    }
    else
    {
        return val;
    }
}

float Maths::LimitVariation(float val, float lastValue, float period, float boundary)
{
    if (val - lastValue > boundary * period)
    {
        return lastValue + boundary * period;
    }
    else if (val - lastValue < -boundary * period)
    {
        return lastValue - boundary * period;
    }
    else
    {
        return val;
    }
}

bool Maths::InsideBoundary(float val, float boundary)
{
    if ((val >= -boundary) && (val <= boundary))
    {
        return true;
    }
    else
    {
        return false;
    }
}

float Maths::DeltaHeading(float currentAngle, float currentX, float currentY, float targetX, float targetY)
{
    float deltaX = targetX - currentX;
    float deltaY = targetY - currentY;
    float angleVoulu = atan2f(deltaY, deltaX);

    return Maths::mod2pi(angleVoulu - currentAngle);
}

float Maths::DeltaHeadingReverse(float currentAngle, float currentX, float currentY, float targetX, float targetY)
{
    float deltaX = targetX - currentX;
    float deltaY = targetY - currentY;
    float angleVoulu = atan2f(deltaY, deltaX);

    return Maths::mod2pi(angleVoulu + M_PI - currentAngle);
}

float Maths::Distance(float x1, float y1, float x2, float y2)
{
    return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
