#include "BlendMaterial.h"

#include "Utilities.h"

HOST BlendMaterial::BlendMaterial(
    const MaterialBase *_material_1, const MaterialBase *_material_2, const Scalar _alpha) {
  this->material_type_ = MaterialType::BlendMaterial;
  this->material_1_ = _material_1;
  this->material_2_ = _material_2;
  this->alpha_ = _alpha;
}

HOST DEVICE Vector3 BlendMaterial::BSDF(
    const Vector3 &_in,
    const Scalar _in_length,
    const Vector3 &_out,
    const Vector3 &_normal) const {
  return this->material_1_->BSDF(_in, _in_length, _out, _normal) * this->alpha_ +
    this->material_2_->BSDF(_in, _in_length, _out, _normal) * (1 - this->alpha_);
}

HOST DEVICE Vector3 BlendMaterial::Emission(const Vector3 &_out, const Vector3 &_normal) const {
  return this->material_1_->Emission(_out, _normal) * this->alpha_ +
    this->material_2_->Emission(_out, _normal) * (1 - this->alpha_);
}

HOST DEVICE bool BlendMaterial::Sample(
    RandomGenerator *_generator,
    const Vector3 &_in,
    const Vector3 &_normal,
    Vector3 &_out,
    Scalar &_pdf) const {
  if (_generator->NextRange(0, 1) < this->alpha_) {
    return this->material_1_->Sample(_generator, _in, _normal, _out, _pdf);
  } else {
    return this->material_2_->Sample(_generator, _in, _normal, _out, _pdf);
  }
}
