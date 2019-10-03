#pragma once

#include "Common.h"
#include "Vector3.h"

class Ray {
public:
	HOST DEVICE Ray();
	HOST DEVICE Ray(const Vector3 &_origin, const Vector3 &_direction);
	// Return the point of going _distance along this ray.
	HOST DEVICE Vector3 Extend(const Scalar _distance) const;
	// Return the distance from the projection of _point onto this ray this->origin.
	HOST DEVICE Scalar Distance(const Vector3 &_point) const;
public:
	Vector3 origin;
	Vector3 direction;
};

HOST std::ostream &operator << (std::ostream &_stream, const Ray &_ray);

template <typename T>
class Intersection {
public:
	Vector3 position;
	Vector3 normal;
	const T *element;
};
