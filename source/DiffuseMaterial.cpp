#include "DiffuseMaterial.h"

#include "Utilities.h"

HOST DiffuseMaterial::DiffuseMaterial(const Vector3 &_reflectance) {
  this->material_type_ = MaterialType::DiffuseMaterial;
  this->reflectance_ = _reflectance;
}

HOST DEVICE Vector3 DiffuseMaterial::BSDF(
    const Vector3 &_in,
    const Scalar _in_length,
    const Vector3 &_out,
    const Vector3 &_normal) const {
  if (_in.Dot(_normal) * _out.Dot(_normal) >= 0) {
    return Vector3(0, 0, 0);
  } else {
    return this->reflectance_ * _normal.Dot(-_in);
  }
}

HOST DEVICE Vector3 DiffuseMaterial::Emission(const Vector3 &_out, const Vector3 &_normal) const {
  return Vector3(0, 0, 0);
}

HOST DEVICE bool DiffuseMaterial::Sample(
    RandomGenerator *_generator,
    const Vector3 &_in,
    const Vector3 &_normal,
    Vector3 &_out,
    Scalar &_pdf) const {
  Vector3 tangent_1;
  Vector3 tangent_2;
  GenerateAxis(_normal, tangent_1, tangent_2);
  Scalar phi = acos(sqrt(_generator->NextRange(0, 1)));
  Scalar theta = _generator->NextRange(0, PI * 2);
  Vector3 tangent = tangent_1 * cos(theta) + tangent_2 * sin(theta);
  _out = _normal * cos(phi) + tangent * sin(phi);
  _pdf = cos(phi);
  return true;
}
