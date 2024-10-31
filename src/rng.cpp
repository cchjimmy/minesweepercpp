#include "rng.hpp"
#include <random>

long RNG::m_seed = 0;

std::default_random_engine RNG::m_randomEngine =
    std::default_random_engine(m_seed);

RNG::RNG(double min, double max) {
  m_distribution = std::uniform_real_distribution(min, max);
}

double RNG::operator()() { return m_distribution(m_randomEngine); }

void RNG::setSeed(long seed) {
  m_seed = seed;
  m_randomEngine.seed(seed);
}
long RNG::getSeed() { return m_seed; }
