#include "GlossyMaterial.h"

#include "Utilities.h"

HOST GlossyMaterial::GlossyMaterial(const Vector3 &_reflectance, const int _exponent) {
  this->material_type_ = MaterialType::GlossyMaterial;
  this->reflectance_ = _reflectance;
  this->exponent_ = _exponent;
}

HOST DEVICE Vector3 GlossyMaterial::BSDF(
    const Vector3 &_in,
    const Scalar _in_length,
    const Vector3 &_out,
    const Vector3 &_normal) const {
  if (_in.Dot(_normal) >= 0 || _out.Dot(_normal) < 0) {
    return Vector3(0, 0, 0);
  } else {
    Vector3 reflect = Reflect(_in, _normal);
    Scalar base = reflect.Dot(_out);
    if (base < 0) {
      base = 0;
    }
    return this->reflectance_ * pow(reflect.Dot(_out), this->exponent_);
  }
}

HOST DEVICE Vector3 GlossyMaterial::Emission(const Vector3 &_out, const Vector3 &_normal) const {
  return Vector3(0, 0, 0);
}

HOST DEVICE bool GlossyMaterial::Sample(
    RandomGenerator *_generator,
    const Vector3 &_in,
    const Vector3 &_normal,
    Vector3 &_out,
    Scalar &_pdf) const {
  Vector3 reflect = Reflect(_in, _normal);
  Vector3 tangent_1;
  Vector3 tangent_2;
  GenerateAxis(reflect, tangent_1, tangent_2);
  do {
    Scalar phi = acos(pow(_generator->NextRange(0, 1), Scalar(1) / Scalar(this->exponent_ + 1)));
    Scalar theta = _generator->NextRange(0, PI * 2);
    Vector3 tangent = tangent_1 * cos(theta) + tangent_2 * sin(theta);
    _out = reflect * cos(phi) + tangent * sin(phi);
    _pdf = pow(cos(phi), this->exponent_);
  } while (_out.Dot(_normal) < 0);
  return true;
}
