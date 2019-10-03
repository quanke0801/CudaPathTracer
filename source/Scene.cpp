#include "Scene.h"

Scene::Scene() {
	this->bvh_ = new BVH<ObjectBase>();
}

Scene::~Scene() {
	for (int i = 0; i < this->materials_.Size(); ++ i) {
		delete this->materials_[i];
	}
	for (int i = 0; i < this->objects_.Size(); ++ i) {
		delete this->objects_[i];
	}
	if (this->bvh_ != nullptr) {
		delete this->bvh_;
	}
}

DiffuseMaterial *Scene::CreateDiffuseMaterial(const Vector3 &_reflectance) {
	DiffuseMaterial *material = new DiffuseMaterial(_reflectance);
	this->materials_.PushBack(material);
	return material;
}

GlossyMaterial *Scene::CreateGlossyMaterial(const Vector3 &_reflectance, const int _exponent) {
	GlossyMaterial *material = new GlossyMaterial(_reflectance, _exponent);
	this->materials_.PushBack(material);
	return material;
}

EmissionMaterial *Scene::CreateEmissionMaterial(const Vector3 &_radiance) {
	EmissionMaterial *material = new EmissionMaterial(_radiance);
	this->materials_.PushBack(material);
	return material;
}

GlassMaterial *Scene::CreateGlassMaterial(const Vector3 &_color, const Scalar &_ior) {
	GlassMaterial *material = new GlassMaterial(_color, _ior);
	this->materials_.PushBack(material);
	return material;
}

BlendMaterial *Scene::CreateBlendMaterial(
		const MaterialBase *_material_1, const MaterialBase *_material_2, const Scalar _alpha) {
	BlendMaterial *material = new BlendMaterial(_material_1, _material_2, _alpha);
	this->materials_.PushBack(material);
	return material;
}

Mesh *Scene::CreateMesh(
		const Pose &_pose,
		const std::string &_file_path,
		const bool _smooth,
		const MaterialBase *_material) {
	Mesh *mesh = new Mesh(this, _pose, _file_path, _smooth, _material);
	this->objects_.PushBack(mesh);
	this->bvh_->Insert(mesh);
	return mesh;
}

Ball *Scene::CreateBall(
		const Scalar _radius, const Vector3 &_center, const MaterialBase *_material) {
	Ball *ball = new Ball(this, _radius, _center, _material);
	this->objects_.PushBack(ball);
	this->bvh_->Insert(ball);
	return ball;
}

Ball *Scene::CreateEclipse(const Pose &_pose, const MaterialBase *_material) {
	Ball *eclipse = new Ball(this, _pose, _material);
	this->objects_.PushBack(eclipse);
	this->bvh_->Insert(eclipse);
	return eclipse;
}

Cylinder *Scene::CreateCylinder(
		const Scalar _radius,
		const Scalar _height,
		const Vector3 &_axis,
		const Vector3 &_center,
		const MaterialBase *_material) {
	Cylinder *cylinder = new Cylinder(this, _radius, _height, _axis, _center, _material);
	this->objects_.PushBack(cylinder);
	this->bvh_->Insert(cylinder);
	return cylinder;
}

void Scene::BuildBVH() {
	if (this->bvh_ != nullptr) {
		delete this->bvh_;
	}
	Vector<ObjectBase *> pointers;
	pointers.Reserve(this->objects_.Size());
	for (int i = 0; i < this->objects_.Size(); ++ i) {
		pointers.PushBack(this->objects_[i]);
	}
	this->bvh_ = new BVH<ObjectBase>(pointers, 0, pointers.Size());
}

void Scene::UpdateObjectPose(const Pose &_pose, ObjectBase *_object) {
	BVH<ObjectBase> *leaf = this->bvh_->SearchElementLeaf(_object);
	_object->pose_ = _pose;
	leaf->UpdateBoundBottomUp();
}

HOST DEVICE bool Scene::RayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const {
	return this->bvh_->RayIntersect(_ray, _intersection);
}
