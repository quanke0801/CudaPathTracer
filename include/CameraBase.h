#pragma once

#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Ray.h"
#include "Pose.h"
#include "Scene.h"

enum class CameraType {
	PinholeCamera,
	None
};

class CameraBase: public Managed {
public:
	HOST CameraBase(const int _resolution_x, const int _resolution_y, const Pose &_pose = Pose());
	HOST DEVICE int ResolutionX() const;
	HOST DEVICE int ResolutionY() const;
	HOST DEVICE Scalar Depth(const Vector3 &_point) const;
	HOST DEVICE Vector2 Project(const Vector3 &_point) const;
	HOST DEVICE Ray Unproject(const Vector2 &_pixel) const;
private:
	HOST DEVICE Vector2 localProject(const Vector3 &_point) const;
	HOST DEVICE Ray localUnproject(const Vector2 &_pixel) const;
protected:
	CameraType camera_type_;
	int resolution_x_;
	int resolution_y_;
	Pose pose_;
};
