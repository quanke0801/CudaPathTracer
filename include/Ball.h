#pragma once

#include "Common.h"
#include "Vector3.h"
#include "Ray.h"
#include "MaterialBase.h"
#include "ObjectBase.h"

// Ball centered at origin with radius 1.
class Ball: public ObjectBase {
  friend class ObjectBase;
public:
  HOST Ball(Scene *_scene, const Scalar _radius, const Vector3 &_center, const MaterialBase *_material);
  // Constructor for eclipse.
  HOST Ball(Scene *_scene, const Pose &_pose, const MaterialBase *_material);
private:
  HOST DEVICE Vector3 localCenter() const;
	HOST DEVICE Pair<Vector3, Vector3> localBound() const;
  HOST DEVICE bool localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const;
private:
  Scalar radius_;
};
