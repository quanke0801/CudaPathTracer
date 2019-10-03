#pragma once

#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Ray.h"

HOST DEVICE Scalar DegreeToRadian(const Scalar _degree);

HOST DEVICE Scalar RadianToDegree(const Scalar _radian);

// Compute normal direction of triangle.
HOST DEVICE Vector3 TriangleNormal(const Vector3 &_point_1, const Vector3 &_point_2, const Vector3 &_point_3);

HOST DEVICE Vector3 TriangleDecomposite(
  const Vector3 &_point_1, const Vector3 &_point_2, const Vector3 &_point_3, const Vector3 &_point);

HOST DEVICE void GenerateAxis(const Vector3 &_axis_1, Vector3 &_axis_2, Vector3 &_axis_3);

HOST DEVICE void GenerateAxis(
  const Vector3 &_axis_1, const Vector3 &_pivot, Vector3 &_axis_2, Vector3 &_axis_3);

HOST DEVICE Vector3 RotateVector(const Vector3 &_vector, const Vector3 &_axis, const Scalar _angle);

// Return a random integer from [_lower, _upper].
HOST int RandomInteger(const int _lower, const int _upper);

HOST Scalar Random(const Scalar _lower, const Scalar _upper);

// Compute ray's intersection with AABB.
HOST DEVICE bool RayBoxIntersect(const Ray &_ray, const Pair<Vector3, Vector3> &_bound);

HOST DEVICE Vector3 Reflect(const Vector3 &_in, const Vector3 &_normal);
