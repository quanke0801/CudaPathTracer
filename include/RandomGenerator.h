#pragma once

#include "Common.h"

class RandomGenerator: public Managed {
public:
  HOST DEVICE RandomGenerator(const unsigned int _seed = 1);
  HOST DEVICE void SetSeed(const unsigned int _seed);
  HOST DEVICE unsigned int Next();
  HOST DEVICE Scalar NextRange(const Scalar _lower, const Scalar _upper);
  HOST DEVICE static constexpr unsigned int Max();
private:
  HOST DEVICE static constexpr unsigned long long int a();
  HOST DEVICE static constexpr unsigned long long int c();
  HOST DEVICE static constexpr unsigned long long int m();
  HOST DEVICE static constexpr unsigned long long int p1();
  HOST DEVICE static constexpr unsigned long long int p2();
  HOST DEVICE static constexpr unsigned long long int q1();
  HOST DEVICE static constexpr unsigned long long int q2();
  HOST DEVICE static constexpr unsigned long long int m1();
  HOST DEVICE static constexpr unsigned long long int m2();
private:
  unsigned long long int x1, x2, x3;
  unsigned long long int y1, y2, y3;
};


