#include "CameraBase.h"

#include "PinholeCamera.h"

HOST CameraBase::CameraBase(const int _resolution_x, const int _resolution_y, const Pose &_pose):
		camera_type_(CameraType::None),
		resolution_x_(_resolution_x),
		resolution_y_(_resolution_y),
		pose_(_pose) {
}

HOST DEVICE int CameraBase::ResolutionX() const {
	return this->resolution_x_;
}

HOST DEVICE int CameraBase::ResolutionY() const {
	return this->resolution_y_;
}

HOST DEVICE Scalar CameraBase::Depth(const Vector3 &_point) const {
	return this->pose_.FromWorldPoint(_point).z;
}

HOST DEVICE Vector2 CameraBase::Project(const Vector3 &_point) const {
	return this->localProject(this->pose_.FromWorldPoint(_point));
}

HOST DEVICE Ray CameraBase::Unproject(const Vector2 &_pixel) const {
	return this->pose_.ToWorldRay(this->localUnproject(_pixel));
}

HOST DEVICE Vector2 CameraBase::localProject(const Vector3 &_point) const {
	switch (this->camera_type_) {
	case CameraType::PinholeCamera:
		return ((const PinholeCamera *)this)->localProject(_point);
	}
	return Vector2();
}

HOST DEVICE Ray CameraBase::localUnproject(const Vector2 &_pixel) const {
	switch (this->camera_type_) {
	case CameraType::PinholeCamera:
		return ((const PinholeCamera *)this)->localUnproject(_pixel);
	}
	return Ray();
}
