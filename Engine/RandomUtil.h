#pragma once
#include <random>

class RandomUtil
{
public:
    static float GetRandomFloat(float min = 0.0f, float max = 1.0f)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    static int GetRandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }

    static std::mt19937 gen;
};


