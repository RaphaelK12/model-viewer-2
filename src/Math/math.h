#pragma once

inline float Map(float value, float min, float max, float newMin, float newMax)
{
    return (value - min) / (max - min) * (newMax - newMin) + newMin;
}