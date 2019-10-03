#include "ObjectBase.h"

#include "Vector3.h"
#include "Scene.h"

#include "Ball.h"
#include "Cylinder.h"
#include "Mesh.h"

HOST ObjectBase::ObjectBase(Scene *_scene, const Pose &_pose, const MaterialBase *_material):
		object_type_(ObjectType::None), scene_(_scene), pose_(_pose), material_(_material) {
}

HOST ObjectBase::~ObjectBase() {
}

HOST void ObjectBase::LocalRotate(const Vector3 &_axis, const Scalar _angle) {
	this->Rotate(this->pose_.ToWorldVector(_axis), _angle);
}

HOST void ObjectBase::Rotate(const Vector3 &_axis, const Scalar _angle) {
	Pose new_pose = this->pose_;
	for (int i = 0; i < 3; ++ i) {
		new_pose.axis[i] = RotateVector(this->pose_.axis[i], _axis, _angle);
	}
	this->scene_->UpdateObjectPose(new_pose, this);
}

HOST void ObjectBase::LocalScale(const Scalar _scale) {
	this->SetLocalScale(this->pose_.scale * _scale);
}

HOST void ObjectBase::LocalScale(const Vector3 &_scale) {
	this->SetLocalScale(this->pose_.scale * _scale);
}

HOST void ObjectBase::SetLocalScale(const Scalar _scale) {
	this->SetLocalScale(Vector3(1, 1, 1) * _scale);
}

HOST void ObjectBase::SetLocalScale(const Vector3 &_scale) {
	Pose new_pose = this->pose_;
	new_pose.scale = _scale;
	this->scene_->UpdateObjectPose(new_pose, this);
}

HOST void ObjectBase::Scale(const Scalar _scale) {
	this->LocalScale(_scale);
}

HOST void ObjectBase::SetScale(const Scalar _scale) {
	this->SetLocalScale(_scale);
}

HOST void ObjectBase::LocalTranslate(const Vector3 &_translation) {
	this->Translate(this->pose_.ToWorldVector(_translation));
}

HOST void ObjectBase::Translate(const Vector3 &_translation) {
	this->SetTranslation(this->pose_.origin + _translation);
}

HOST void ObjectBase::SetTranslation(const Vector3 &_translation) {
	Pose new_pose = this->pose_;
	new_pose.origin = _translation;
	this->scene_->UpdateObjectPose(new_pose, this);
}

HOST DEVICE const MaterialBase *ObjectBase::Material() const {
	return this->material_;
}

HOST DEVICE Vector3 ObjectBase::Center() const {
	return this->pose_.ToWorldPoint(this->localCenter());
}

HOST DEVICE Pair<Vector3, Vector3> ObjectBase::Bound() const {
	Pair<Vector3, Vector3> local_bound = this->localBound();
	Vector3 center = this->Center();
	Pair<Vector3, Vector3> bound(center, center);
	for (int i = 0; i < 8; ++ i) {
		Vector3 local_vertex;
		local_vertex.x = (i & 1) ? local_bound.first.x : local_bound.second.x;
		local_vertex.y = (i & 2) ? local_bound.first.y : local_bound.second.y;
		local_vertex.z = (i & 4) ? local_bound.first.z : local_bound.second.z;
		Vector3 vertex = this->pose_.ToWorldPoint(local_vertex);
		bound.first = Min(bound.first, vertex);
		bound.second = Max(bound.second, vertex);
	}
	return bound;
}

HOST DEVICE bool ObjectBase::RayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const {
	Ray local_ray = this->pose_.FromWorldRay(_ray);
	bool intersect = this->localRayIntersect(local_ray, _intersection);
	if (intersect) {
		_intersection.position = this->pose_.ToWorldPoint(_intersection.position);
		_intersection.normal = this->pose_.ToWorldVector(_intersection.normal).Normalized();
	}
	return intersect;
}

HOST DEVICE Vector3 ObjectBase::localCenter() const {
	switch (this->object_type_) {
	case ObjectType::Ball:
		return ((const Ball *)this)->localCenter();
	case ObjectType::Cylinder:
		return ((const Cylinder *)this)->localCenter();
	case ObjectType::Mesh:
		return ((const Mesh *)this)->localCenter();
	}
	return Vector3();
}

HOST DEVICE Pair<Vector3, Vector3> ObjectBase::localBound() const {
	switch (this->object_type_) {
	case ObjectType::Ball:
		return ((const Ball *)this)->localBound();
	case ObjectType::Cylinder:
		return ((const Cylinder *)this)->localBound();
	case ObjectType::Mesh:
		return ((const Mesh *)this)->localBound();
	}
	return Pair<Vector3, Vector3>();
}

HOST DEVICE bool ObjectBase::localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const {
	switch (this->object_type_) {
	case ObjectType::Ball:
		return ((const Ball *)this)->localRayIntersect(_ray, _intersection);
	case ObjectType::Cylinder:
		return ((const Cylinder *)this)->localRayIntersect(_ray, _intersection);
	case ObjectType::Mesh:
		return ((const Mesh *)this)->localRayIntersect(_ray, _intersection);
	}
	return false;
}
