#include "Pose.h"

HOST DEVICE Pose::Pose() {
	this->setScaleOrigin(Vector3(1, 1, 1), Vector3(0, 0, 0));
}

HOST DEVICE Pose::Pose(const Vector3 &_origin) {
	this->setScaleOrigin(Vector3(1, 1, 1), _origin);
}

HOST DEVICE Pose::Pose(const Scalar _scale, const Vector3 &_origin) {
	this->setScaleOrigin(Vector3(_scale, _scale, _scale), _origin);
}

HOST DEVICE Pose::Pose(const Vector3 &_scale, const Vector3 &_origin) {
	this->setScaleOrigin(_scale, _origin);
}

HOST DEVICE Pose::Pose(const Vector3 _axis[3], const Vector3 &_scale, const Vector3 &_origin) {
	this->setScaleOrigin(_scale, _origin);
	for (int i = 0; i < 3; ++ i) {
		this->axis[i] = _axis[i].Normalized();
	}
}

HOST DEVICE Pose Pose::Composite(const Pose &_child) const {
	Pose composition;
	for (int i = 0; i < 3; ++ i) {
		composition.axis[i] = this->ToWorldVector(_child.axis[i]);
	}
	composition.origin = this->ToWorldPoint(_child.origin);
	return composition;
}

HOST DEVICE Vector3 Pose::ToWorldPoint(const Vector3 &_point) const {
	return this->origin + this->ToWorldVector(_point);
}

HOST DEVICE Vector3 Pose::ToWorldVector(const Vector3 &_vector) const {
	Vector3 scaled = _vector * this->scale;
	return this->axis[0] * scaled.x + this->axis[1] * scaled.y + this->axis[2] * scaled.z;
}

HOST DEVICE Ray Pose::ToWorldRay(const Ray &_ray) const {
	return Ray(this->ToWorldPoint(_ray.origin), this->ToWorldVector(_ray.direction));
}

HOST DEVICE Vector3 Pose::FromWorldPoint(const Vector3 &_point) const {
	return this->FromWorldVector(_point - this->origin);
}

HOST DEVICE Vector3 Pose::FromWorldVector(const Vector3 &_vector) const {
	return Vector3(_vector.Dot(this->axis[0]), _vector.Dot(this->axis[1]), _vector.Dot(this->axis[2])) / this->scale;
}

HOST DEVICE Ray Pose::FromWorldRay(const Ray &_ray) const {
	return Ray(this->FromWorldPoint(_ray.origin), this->FromWorldVector(_ray.direction));
}

HOST DEVICE void Pose::setScaleOrigin(const Vector3 &_scale, const Vector3 &_origin) {
	this->axis[0] = Vector3(1, 0, 0);
	this->axis[1] = Vector3(0, 1, 0);
	this->axis[2] = Vector3(0, 0, 1);
	this->scale = _scale;
	this->origin = _origin;
}

HOST std::ostream &operator << (std::ostream &_stream, const Pose &_pose) {
	_stream << "{" << _pose.axis[0] << _pose.axis[1] << _pose.axis[2] << "}" << _pose.scale << _pose.origin;
	return _stream;
}
