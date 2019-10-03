#include "MaterialBase.h"

#include "BlendMaterial.h"
#include "DiffuseMaterial.h"
#include "EmissionMaterial.h"
#include "GlassMaterial.h"
#include "GlossyMaterial.h"

HOST MaterialBase::MaterialBase(): material_type_(MaterialType::None) {
}

HOST MaterialBase::~MaterialBase() {
}

HOST DEVICE Vector3 MaterialBase::BSDF(
    const Vector3 &_in,
    const Scalar _in_length,
    const Vector3 &_out,
    const Vector3 &_normal) const {
  switch (this->material_type_) {
  case MaterialType::BlendMaterial:
    return ((const BlendMaterial *)this)->BSDF(_in, _in_length, _out, _normal);
  case MaterialType::DiffuseMaterial:
    return ((const DiffuseMaterial *)this)->BSDF(_in, _in_length, _out, _normal);
  case MaterialType::EmissionMaterial:
    return ((const EmissionMaterial *)this)->BSDF(_in, _in_length, _out, _normal);
  case MaterialType::GlassMaterial:
    return ((const GlassMaterial *)this)->BSDF(_in, _in_length, _out, _normal);
  case MaterialType::GlossyMaterial:
    return ((const GlossyMaterial *)this)->BSDF(_in, _in_length, _out, _normal);
  }
  return Vector3();
}

HOST DEVICE Vector3 MaterialBase::Emission(
    const Vector3 &_out, const Vector3 &_normal) const {
  switch (this->material_type_) {
  case MaterialType::BlendMaterial:
    return ((const BlendMaterial *)this)->Emission(_out, _normal);
  case MaterialType::DiffuseMaterial:
    return ((const DiffuseMaterial *)this)->Emission(_out, _normal);
  case MaterialType::EmissionMaterial:
    return ((const EmissionMaterial *)this)->Emission(_out, _normal);
  case MaterialType::GlassMaterial:
    return ((const GlassMaterial *)this)->Emission(_out, _normal);
  case MaterialType::GlossyMaterial:
    return ((const GlossyMaterial *)this)->Emission(_out, _normal);
  }
  return Vector3();
}

HOST DEVICE bool MaterialBase::Sample(
    RandomGenerator *_generator,
    const Vector3 &_in,
    const Vector3 &_normal,
    Vector3 &_out,
    Scalar &_pdf) const {
  switch (this->material_type_) {
  case MaterialType::BlendMaterial:
    return ((const BlendMaterial *)this)->Sample(_generator, _in, _normal, _out, _pdf);
  case MaterialType::DiffuseMaterial:
    return ((const DiffuseMaterial *)this)->Sample(_generator, _in, _normal, _out, _pdf);
  case MaterialType::EmissionMaterial:
    return ((const EmissionMaterial *)this)->Sample(_generator, _in, _normal, _out, _pdf);
  case MaterialType::GlassMaterial:
    return ((const GlassMaterial *)this)->Sample(_generator, _in, _normal, _out, _pdf);
  case MaterialType::GlossyMaterial:
    return ((const GlossyMaterial *)this)->Sample(_generator, _in, _normal, _out, _pdf);
  }
  return false;
}

