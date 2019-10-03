#pragma once

#include "Common.h"
#include "Vector3.h"
#include "Ray.h"

// Pose class to handle object rotation, scaling and translation.
// By separating these 3 parts it becomes much clearer than using matrix multiplication.
class Pose {
public:
	HOST DEVICE Pose();
	HOST DEVICE Pose(const Vector3 &_origin);
	HOST DEVICE Pose(const Scalar _scale, const Vector3 &_origin);
	HOST DEVICE Pose(const Vector3 &_scale, const Vector3 &_origin);
	HOST DEVICE Pose(const Vector3 _axis[3], const Vector3 &_scale, const Vector3 &_origin);
	// Compute the composition pose of *this and _child.
	HOST DEVICE Pose Composite(const Pose &_child) const;
	// Conversions.
	HOST DEVICE Vector3 ToWorldPoint(const Vector3 &_point) const;
	HOST DEVICE Vector3 ToWorldVector(const Vector3 &_vector) const;
	HOST DEVICE Ray ToWorldRay(const Ray &_ray) const;
	HOST DEVICE Vector3 FromWorldPoint(const Vector3 &_point) const;
	HOST DEVICE Vector3 FromWorldVector(const Vector3 &_vector) const;
	HOST DEVICE Ray FromWorldRay(const Ray &_ray) const;
public:
	Vector3 axis[3];
	Vector3 scale;
	Vector3 origin;
private:
	HOST DEVICE void setScaleOrigin(const Vector3 &_scale, const Vector3 &_origin);
};

HOST std::ostream &operator << (std::ostream &_stream, const Pose &_pose);
