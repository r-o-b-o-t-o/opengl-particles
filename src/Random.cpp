#include <ctime>

#include "Random.h"

std::mt19937 Random::rng(static_cast<unsigned int>(time(nullptr)));

int Random::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(Random::rng);
}

float Random::getFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(Random::rng);
}

double Random::getDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(Random::rng);
}

bool Random::getBool() {
	return getInt(0, 1) == 1;
}
