#include "PinholeCamera.h"

HOST PinholeCamera::PinholeCamera(
    const int _resolution_x,
    const int _resolution_y,
    const Pose &_pose,
    const Scalar _fov):
    CameraBase(_resolution_x, _resolution_y, _pose) {
  this->camera_type_ = CameraType::PinholeCamera;
  this->fov_ = _fov;
  this->pixel_center_ = Vector2(this->resolution_x_, this->resolution_y_) / 2;
  this->pixel_size_ = std::tan(this->fov_ / 2) / this->resolution_x_ * 2;
}

HOST DEVICE Vector2 PinholeCamera::localProject(const Vector3 &_point) const {
  // TODO
  return Vector2(0, 0);
}

HOST DEVICE Ray PinholeCamera::localUnproject(const Vector2 &_pixel) const {
  return Ray(Vector3(0, 0, 0), Vector3(
    (_pixel.x - this->pixel_center_.x) * this->pixel_size_,
    (_pixel.y - this->pixel_center_.y) * this->pixel_size_,
    1));
}
