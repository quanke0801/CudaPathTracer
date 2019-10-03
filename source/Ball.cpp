#include "Ball.h"

HOST Ball::Ball(
    Scene *_scene, const Scalar _radius, const Vector3 &_center, const MaterialBase *_material):
    ObjectBase(_scene, Pose(_radius, _center), _material) {
  this->object_type_ = ObjectType::Ball;
}

HOST Ball::Ball(Scene *_scene, const Pose &_pose, const MaterialBase *_material):
    ObjectBase(_scene, _pose, _material) {
}

HOST DEVICE Vector3 Ball::localCenter() const {
  return Vector3(0, 0, 0);
}

HOST DEVICE Pair<Vector3, Vector3> Ball::localBound() const {
  return Pair<Vector3, Vector3>(Vector3(-1, -1, -1), Vector3(1, 1, 1));
}

HOST DEVICE bool Ball::localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const {
  // Equation is (_ray.origin + t * _ray.direction)^2 = 1.
  Scalar b = _ray.origin.Dot(_ray.direction) * 2;
  Scalar c = _ray.origin.LengthSquare() - 1;
  Scalar delta = b * b - c * 4;
  if (delta < 0) {
    return false;
  } else {
    Scalar root_delta = std::sqrt(delta);
    Scalar t1 = (-b - root_delta) / 2;
    Scalar t2 = (-b + root_delta) / 2;
    if (t2 >= SELF_INTERSECT_THRESHOLD) {
      _intersection.position = _ray.Extend(t1 >= SELF_INTERSECT_THRESHOLD ? t1 : t2);
      _intersection.normal = _intersection.position;
      _intersection.element = this;
      return true;
    } else {
      return false;
    }
  }
}
