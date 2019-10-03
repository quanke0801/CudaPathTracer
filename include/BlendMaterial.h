#pragma once

#include "Vector3.h"
#include "MaterialBase.h"

class BlendMaterial: public MaterialBase {
  friend class MaterialBase;
public:
  HOST BlendMaterial(
    const MaterialBase *_material_1, const MaterialBase *_material_2, const Scalar _alpha);
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
  const MaterialBase *material_1_;
  const MaterialBase *material_2_;
  Scalar alpha_;
};
