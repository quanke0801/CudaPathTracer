#include "EmissionMaterial.h"

HOST EmissionMaterial::EmissionMaterial(const Vector3 &_radiance) {
  this->material_type_ = MaterialType::EmissionMaterial;
  this->radiance_ = _radiance;
}

HOST DEVICE Vector3 EmissionMaterial::BSDF(
    const Vector3 &_in,
    const Scalar _in_length,
    const Vector3 &_out,
    const Vector3 &_normal) const {
  return Vector3(0, 0, 0);
}

HOST DEVICE Vector3 EmissionMaterial::Emission(const Vector3 &_out, const Vector3 &_normal) const {
  // TODO Change to direction/distance dependent emission.
  return this->radiance_;
}

HOST DEVICE bool EmissionMaterial::Sample(
    RandomGenerator *_generator,
    const Vector3 &_in,
    const Vector3 &_normal,
    Vector3 &_out,
    Scalar &_pdf) const {
  return false;
}
