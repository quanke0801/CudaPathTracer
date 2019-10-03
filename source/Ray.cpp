#include "Ray.h"

#include "Mesh.h"
#include "ObjectBase.h"

HOST DEVICE Ray::Ray() {
}

HOST DEVICE Ray::Ray(const Vector3 &_origin, const Vector3 &_direction):
		origin(_origin),
		direction(_direction.Normalized()) {
}

HOST DEVICE Vector3 Ray::Extend(const Scalar _distance) const {
	return this->origin + this->direction * _distance;
}

HOST DEVICE Scalar Ray::Distance(const Vector3 &_point) const {
	return (_point - this->origin).Length();
}

HOST std::ostream &operator << (std::ostream &_stream, const Ray &_ray) {
	_stream << _ray.origin << "->" << _ray.direction;
	return _stream;
}

template class Intersection<MeshTriangle>;
template class Intersection<ObjectBase>;
