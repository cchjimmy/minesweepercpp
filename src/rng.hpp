#pragma once

#include <random>

class RNG {
public:
  RNG(double min = 0, double max = 1);
  double operator()();
  void setSeed(long seed);
  long getSeed();

private:
  static long m_seed;
  static std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<double> m_distribution;
};
