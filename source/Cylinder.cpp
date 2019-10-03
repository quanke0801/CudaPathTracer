#include "Cylinder.h"

#include "Utilities.h"

HOST Cylinder::Cylinder(
    Scene *_scene,
    const Scalar _radius,
    const Scalar _height,
    const Vector3 &_axis,
    const Vector3 &_center,
    const MaterialBase *_material):
    ObjectBase(_scene, Pose(), _material) {
  this->object_type_ = ObjectType::Cylinder;
  this->pose_.axis[2] = _axis;
  GenerateAxis(this->pose_.axis[2], this->pose_.axis[0], this->pose_.axis[1]);
  this->pose_.scale = Vector3(_radius, _radius, _height);
  this->pose_.origin = _center;
}

HOST DEVICE Vector3 Cylinder::localCenter() const {
  return Vector3(0, 0, 0);
}

HOST DEVICE Pair<Vector3, Vector3> Cylinder::localBound() const {
  return Pair<Vector3, Vector3>(Vector3(-1, -1, -0.5), Vector3(1, 1, 0.5));
}

HOST DEVICE bool Cylinder::localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const {
  Scalar t = -1;
  Vector3 normal;
  // Test top circle.
  Scalar top_t = (Scalar(0.5) - _ray.origin.z) / _ray.direction.z;
  Vector3 top_hit = _ray.Extend(top_t);
  if (top_t >= 0 && Vector2(top_hit.x, top_hit.y).Length() <= 1 && (t < 0 || top_t < t)) {
    t = top_t;
    normal = Vector3(0, 0, 1);
  }
  // Test bottom circle.
  Scalar bottom_t = (Scalar(-0.5) - _ray.origin.z) / _ray.direction.z;
  Vector3 bottom_hit = _ray.Extend(bottom_t);
  if (bottom_t >= 0 && Vector2(bottom_hit.x, bottom_hit.y).Length() <= 1 && (t < 0 || bottom_t < t)) {
    t = bottom_t;
    normal = Vector3(0, 0, -1);
  }
  // Test side curve surface.
  Vector2 origin_2d(_ray.origin.x, _ray.origin.y);
  Vector2 direction_2d(_ray.direction.x, _ray.direction.y);
  Scalar a = direction_2d.LengthSquare();
  Scalar b = origin_2d.Dot(direction_2d) * 2;
  Scalar c = origin_2d.LengthSquare() - 1;
  Scalar delta = b * b - 4 * a * c;
  if (delta >= 0) {
    Scalar root_delta = std::sqrt(delta);
    Scalar side_t_1 = (-b + root_delta) / a / 2;
    Vector3 side_hit_1 = _ray.Extend(side_t_1);
    if (side_t_1 >= SELF_INTERSECT_THRESHOLD &&
        std::abs(side_hit_1.z) <= -0.5 &&
        (t < 0 || side_t_1 < t)) {
      t = side_t_1;
      normal = Vector3(side_hit_1.x, side_hit_1.y, 0);
    }
    Scalar side_t_2 = (-b - root_delta) / a / 2;
    Vector3 side_hit_2 = _ray.Extend(side_t_2);
    if (side_t_2 >= SELF_INTERSECT_THRESHOLD &&
        std::abs(side_hit_2.z) <= 0.5 &&
        (t < 0 || side_t_2 < t)) {
      t = side_t_2;
      normal = Vector3(side_hit_2.x, side_hit_2.y, 0);
    }
  }
  if (t >= SELF_INTERSECT_THRESHOLD) {
    _intersection.position = _ray.Extend(t);
    _intersection.normal = normal;
    _intersection.element = this;
    return true;
  } else {
    return false;
  }
}
