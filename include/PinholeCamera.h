#pragma once

#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Ray.h"
#include "CameraBase.h"

class PinholeCamera: public CameraBase {
  friend class CameraBase;
public:
  HOST PinholeCamera(
    const int _resolution_x,
    const int _resolution_y,
    const Pose &_pose,
    const Scalar _fov);
private:
  HOST DEVICE Vector2 localProject(const Vector3 &_point) const;
	HOST DEVICE Ray localUnproject(const Vector2 &_pixel) const;
private:
  Scalar fov_;
  Vector2 pixel_center_;
  Scalar pixel_size_;
};
