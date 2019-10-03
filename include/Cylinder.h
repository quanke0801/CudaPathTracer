#pragma once

#include "Common.h"
#include "Vector3.h"
#include "Ray.h"
#include "MaterialBase.h"
#include "ObjectBase.h"

// Cylinder centered at origin with radius 1 and height 1, and z is height direction.
class Cylinder: public ObjectBase {
  friend class ObjectBase;
public:
  HOST Cylinder(
    Scene *_scene,
    const Scalar _radius,
    const Scalar _height,
    const Vector3 &_axis,
    const Vector3 &_center,
    const MaterialBase *_material);
private:
  HOST DEVICE Vector3 localCenter() const;
	HOST DEVICE Pair<Vector3, Vector3> localBound() const;
  HOST DEVICE bool localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const;
};
