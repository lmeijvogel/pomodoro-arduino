#include <math.h>
#include <stdlib.h>

#include "RandomGenerator.h"

const long MAX_RANDOM = 32768;
const double INVERSE_MAX_RANDOM = 1.0 / MAX_RANDOM;

RandomGenerator::RandomGenerator(int seed) {
  srand(seed);
}

long RandomGenerator::getNext(long max) {
  // & 32767 since that's the max output on an Arduino
  double between_zero_and_one = (rand() & 32767) * INVERSE_MAX_RANDOM;
  return between_zero_and_one * max;
}

float RandomGenerator::getNextPoisson(int rateParameter) {
  return -logf(1.0f - (float) rand() / (RAND_MAX)) / (float)rateParameter;
}
