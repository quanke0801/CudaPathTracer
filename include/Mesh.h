#pragma once

#include "Common.h"
#include "Vector3.h"
#include "Ray.h"
#include "Pose.h"
#include "MaterialBase.h"
#include "ObjectBase.h"
#include "BVH.h"

class MeshVertex {
public:
	HOST DEVICE MeshVertex();
	Vector3 position;
	Vector3 normal;
};

class MeshTriangle {
public:
	HOST DEVICE MeshTriangle();
	HOST DEVICE void ComputeNormal();
	HOST DEVICE Scalar Area() const;
	HOST DEVICE bool RayIntersect(const Ray &_ray, Intersection<MeshTriangle> &_intersection) const;
	HOST DEVICE Vector3 Center() const;
	HOST DEVICE Pair<Vector3, Vector3> Bound() const;
public:
	MeshVertex *vertices[3];
	Vector3 normal;
};

// Class to hold mesh data and map mesh path to existing MeshData object to avoid loading again.
// The mesh data and bvh are loaded and computed and then immutable.
// Multiple mesh objects can thus be created efficiently and positioned by controlling Mesh::pose_.
class MeshData: public Managed {
	friend class Mesh;
public:
	HOST MeshData(const std::string &_path);
	HOST ~MeshData();
public:
	// Get a MeshData pointer which holds the data of mesh at _path.
	HOST static MeshData *Load(const std::string &_path);
public:
	// Map mesh path to MeshData instance.
	static std::map<std::string, MeshData *> DATA_;
private:
	// Read mesh file at _path, load vertices and triangles and build bvh.
	HOST void load(const std::string &_path);
private:
	Vector<MeshVertex> vertices_;
	Vector<MeshTriangle> triangles_;
	BVH<MeshTriangle> *bvh_;
};

// Mesh object.
class Mesh: public ObjectBase {
	friend class ObjectBase;
public:
	HOST Mesh(
		Scene *_scene,
		const Pose &_pose,
		const std::string &_path,
		const bool _smooth,
		const MaterialBase *_material);
private:
	HOST DEVICE Vector3 localCenter() const;
	HOST DEVICE Pair<Vector3, Vector3> localBound() const;
	HOST DEVICE bool localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const;
private:
	MeshData *data_;
	// If smooth_ is true, vertex normals will be used instead of triangle normals.
	bool smooth_;
};
