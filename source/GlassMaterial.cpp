#include "GlassMaterial.h"

#include "Utilities.h"

HOST GlassMaterial::GlassMaterial(const Vector3 &_color, const Scalar &_ior) {
  this->material_type_ = MaterialType::GlassMaterial;
  this->absorbtion_exponent_ = Vector3(
    -std::log(_color.x), -std::log(_color.y), -std::log(_color.z));
  this->ior_ = _ior;
}

HOST DEVICE Vector3 GlassMaterial::BSDF(
    const Vector3 &_in,
    const Scalar _in_length,
    const Vector3 &_out,
    const Vector3 &_normal) const {
  Vector3 bsdf;
  bool in_enter = _in.Dot(_normal) < 0;
  bool out_enter = _out.Dot(_normal) < 0;
  Vector3 refract;
  Scalar refract_ratio;
  this->computeRefract(
    _in, _normal, in_enter ? this->ior_ : 1 / this->ior_, refract, refract_ratio);
  if (in_enter == out_enter) {
    // Refraction.
    if (refract.Cross(_out).Length() < SCALAR_LOOSE_DELTA) {
      bsdf = Vector3(refract_ratio, refract_ratio, refract_ratio);
    } else {
      bsdf = Vector3(0, 0, 0);
    }
  } else {
    // Reflection.
    if (Reflect(_in, _normal).Cross(_out).Length() < SCALAR_LOOSE_DELTA) {
      bsdf = Vector3(1 - refract_ratio, 1 - refract_ratio, 1 - refract_ratio);
    } else {
      bsdf = Vector3(0, 0, 0);
    }
  }
  if (!in_enter) {
    bsdf *= Vector3(
      std::exp(-this->absorbtion_exponent_.x * _in_length),
      std::exp(-this->absorbtion_exponent_.y * _in_length),
      std::exp(-this->absorbtion_exponent_.z * _in_length));
  }
  return bsdf;
}

HOST DEVICE Vector3 GlassMaterial::Emission(const Vector3 &_out, const Vector3 &_normal) const {
  return Vector3(0, 0, 0);
}

HOST DEVICE bool GlassMaterial::Sample(
    RandomGenerator *_generator,
    const Vector3 &_in,
    const Vector3 &_normal,
    Vector3 &_out,
    Scalar &_pdf) const {
  bool in_enter = _in.Dot(_normal) < 0;
  Vector3 refract;
  Scalar refract_ratio;
  this->computeRefract(
    _in, _normal, in_enter ? this->ior_ : 1 / this->ior_, refract, refract_ratio);
  if (_generator->NextRange(0, 1) < refract_ratio) {
    // Sample refracted ray.
    _out = refract;
    _pdf = refract_ratio;
  } else {
    // Sample reflected ray.
    _out = Reflect(_in, _normal);
    _pdf = 1 - refract_ratio;
  }
  return true;
}

HOST DEVICE bool GlassMaterial::computeRefract(
    const Vector3 &_in,
    const Vector3 &_normal,
    const Scalar _ior,
    Vector3 &_out,
    Scalar &_ratio) const {
	Scalar cos_in = _in.Dot(- _normal);
	Scalar sin_in = sqrt(1 - cos_in * cos_in);
	Scalar sin_out = sin_in * _ior;
	if (sin_out >= 1) {
		_ratio = 0;
		return false;
	}
	Scalar cos_out = sqrt(1 - sin_out * sin_out);
	Vector3 tangent1 = _in.Cross(_normal).Normalized();
	Vector3 tangent2 = _normal.Cross(tangent1).Normalized();
	_out = tangent2 * sin_out - _normal * cos_out;
	Scalar root_reflectance_s = (_ior * cos_in - cos_out) / (_ior * cos_in + cos_out);
	Scalar root_reflectance_p = (_ior * cos_out - cos_in) / (_ior * cos_out + cos_in);
	_ratio = 1 - root_reflectance_s * root_reflectance_s - root_reflectance_p * root_reflectance_p;
  return true;
}
