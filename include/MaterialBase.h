#pragma once

#include "Common.h"
#include "Vector3.h"
#include "RandomGenerator.h"

enum class MaterialType {
  BlendMaterial,
  DiffuseMaterial,
  EmissionMaterial,
  GlassMaterial,
  GlossyMaterial,
  None
};

class MaterialBase: public Managed {
public:
  HOST MaterialBase();
  HOST virtual ~MaterialBase();
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
protected:
  MaterialType material_type_;
};
