#pragma once

#include "Vector3.h"
#include "MaterialBase.h"

class EmissionMaterial: public MaterialBase {
  friend class MaterialBase;
public:
  HOST EmissionMaterial(const Vector3 &_radiance);
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
  Vector3 radiance_;
};
