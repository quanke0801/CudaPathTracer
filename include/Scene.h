#pragma once

#include "Common.h"
#include "Pose.h"
#include "MaterialBase.h"
#include "DiffuseMaterial.h"
#include "GlossyMaterial.h"
#include "EmissionMaterial.h"
#include "GlassMaterial.h"
#include "BlendMaterial.h"
#include "ObjectBase.h"
#include "Mesh.h"
#include "Ball.h"
#include "Cylinder.h"
#include "BVH.h"

// Scene class containing 3D objects.
class Scene: public Managed {
public:
	Scene();
	~Scene();
	// Creation of materials.
	DiffuseMaterial *CreateDiffuseMaterial(const Vector3 &_reflectance);
	GlossyMaterial *CreateGlossyMaterial(const Vector3 &_reflectance, const int _exponent);
	EmissionMaterial *CreateEmissionMaterial(const Vector3 &_radiance);
	GlassMaterial *CreateGlassMaterial(const Vector3 &_absorbtion, const Scalar &_ior);
	BlendMaterial *CreateBlendMaterial(
		const MaterialBase *_material_1, const MaterialBase *_material_2, const Scalar _alpha);
	// Creation of objects.
	Mesh *CreateMesh(
		const Pose &_pose,
		const std::string &_file_path,
		const bool _smooth,
		const MaterialBase *_material);
	Ball *CreateBall(const Scalar _radius, const Vector3 &_center, const MaterialBase *_material);
	Ball *CreateEclipse(const Pose &_pose, const MaterialBase *_material);
	Cylinder *CreateCylinder(
		const Scalar _radius,
		const Scalar _height,
		const Vector3 &_axis,
		const Vector3 &_center,
		const MaterialBase *_material);
	// Build bvh for existing objects.
	void BuildBVH();
	// Update bvh bottom up when object is moved.
	void UpdateObjectPose(const Pose &_pose, ObjectBase *_object);
	HOST DEVICE bool RayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const;
private:
	Vector<MaterialBase *> materials_;
	Vector<ObjectBase *> objects_;
	BVH<ObjectBase> *bvh_;
};
