#pragma once

#include "Vector3.h"
#include "MaterialBase.h"

// Glass material. Argument _color is the visual color when white light goes inside material for
// 1 meter.
// The model is output_radiance = input_radiance * exp(-c * distance), where c is constant.
// To ensure exp(-c * distance) evaluates to _color when distance is 1, c = -ln(_color).
class GlassMaterial: public MaterialBase {
  friend class MaterialBase;
public:
  HOST GlassMaterial(const Vector3 &_color, const Scalar &_ior);
  HOST DEVICE Vector3 BSDF(
    const Vector3 &_in,
    const Scalar _in_length,
    const Vector3 &_out,
    const Vector3 &_normal) const;
  HOST DEVICE Vector3 Emission(const Vector3 &_out, const Vector3 &_normal) const;
  HOST DEVICE bool Sample(
    RandomGenerator *_generator,
    const Vector3 &_in,
    const Vector3 &_normal,
    Vector3 &_out,
    Scalar &_pdf) const;
private:
  HOST DEVICE bool computeRefract(
    const Vector3 &_in,
    const Vector3 &_normal,
    const Scalar _ior,
    Vector3 &_out,
    Scalar &_ratio) const;
private:
  Vector3 absorbtion_exponent_;
  Scalar ior_;
};
