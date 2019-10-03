#pragma once

#include "Common.h"
#include "Vector3.h"
#include "Ray.h"
#include "Pose.h"
#include "MaterialBase.h"

class Scene;

enum class ObjectType {
	Ball,
	Cylinder,
	Mesh,
	None
};

// Base class of all ray-intersectable 3d objects.
class ObjectBase: public Managed {
	friend class Scene;
public:
	// Store _scene update bound and notify scene to update bvh.
	HOST ObjectBase(Scene *_scene, const Pose &_pose, const MaterialBase *_material);
	HOST virtual ~ObjectBase();
	// Transformations.
	HOST void LocalRotate(const Vector3 &_axis, const Scalar _angle);
	HOST void Rotate(const Vector3 &_axis, const Scalar _angle);
	HOST void LocalScale(const Scalar _scale);
	HOST void LocalScale(const Vector3 &_scale);
	HOST void SetLocalScale(const Scalar _scale);
	HOST void SetLocalScale(const Vector3 &_scale);
	HOST void Scale(const Scalar _scale);
	HOST void SetScale(const Scalar _scale);
	HOST void LocalTranslate(const Vector3 &_translation);
	HOST void Translate(const Vector3 &_translation);
	HOST void SetTranslation(const Vector3 &_translation);
	// Get material.
	HOST DEVICE const MaterialBase *Material() const;
	// Get world-space center and bounds by converting local center and bounds.
	HOST DEVICE Vector3 Center() const;
	HOST DEVICE Pair<Vector3, Vector3> Bound() const;
	// Convert _ray to local space and call localRayIntersect().
	HOST DEVICE bool RayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const;
private:
	HOST DEVICE Vector3 localCenter() const;
	HOST DEVICE Pair<Vector3, Vector3> localBound() const;
	HOST DEVICE bool localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const;
protected:
	ObjectType object_type_;
	Scene *scene_;
	Pose pose_;
	const MaterialBase *material_;
};
