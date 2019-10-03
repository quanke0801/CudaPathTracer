#include "RandomGenerator.h"

HOST DEVICE RandomGenerator::RandomGenerator(const unsigned int _seed) {
  this->SetSeed(_seed);
}

HOST DEVICE void RandomGenerator::SetSeed(const unsigned int _seed) {
  unsigned long long int seed = _seed;
  this->x1 = (seed * RandomGenerator::a() + RandomGenerator::c()) % RandomGenerator::m();
  this->x2 = (seed * RandomGenerator::a() + RandomGenerator::c()) % RandomGenerator::m();
  this->x3 = (seed * RandomGenerator::a() + RandomGenerator::c()) % RandomGenerator::m();
  this->y1 = (seed * RandomGenerator::a() + RandomGenerator::c()) % RandomGenerator::m();
  this->y2 = (seed * RandomGenerator::a() + RandomGenerator::c()) % RandomGenerator::m();
  this->y3 = (seed * RandomGenerator::a() + RandomGenerator::c()) % RandomGenerator::m();
}

HOST DEVICE unsigned int RandomGenerator::Next() {
  unsigned long long int new_x =
    (RandomGenerator::p1() * this->x2 - RandomGenerator::p2() * this->x3) % RandomGenerator::m1();
  unsigned long long int new_y =
    (RandomGenerator::q1() * this->y1 - RandomGenerator::q2() * this->y3) % RandomGenerator::m2();
  this->x3 = this->x2;
  this->x2 = this->x1;
  this->x1 = new_x;
  this->y3 = this->y2;
  this->y2 = this->y1;
  this->y1 = new_y;
  unsigned long long int random = (new_x + RandomGenerator::m1() - new_y) % RandomGenerator::m1();
  return (unsigned int)random;
}

HOST DEVICE Scalar RandomGenerator::NextRange(const Scalar _lower, const Scalar _upper) {
  return Scalar(this->Next()) / Scalar(RandomGenerator::Max()) * (_upper - _lower) + _lower;
}

HOST DEVICE constexpr unsigned int RandomGenerator::Max() {
  return (unsigned int)RandomGenerator::m1();
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::a() {
  return 16807;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::c() {
  return 0;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::m() {
  return 2147483647;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::p1() {
  return 1403580;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::p2() {
  return 810728;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::q1() {
  return 527612;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::q2() {
  return 1370589;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::m1() {
  return 4294967087;
}

HOST DEVICE constexpr unsigned long long int RandomGenerator::m2() {
  return 4294944713;
}
