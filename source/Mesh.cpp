#include "Mesh.h"

#include "Common.h"

HOST DEVICE MeshVertex::MeshVertex() {
}

HOST DEVICE MeshTriangle::MeshTriangle() {
}

HOST DEVICE void MeshTriangle::ComputeNormal() {
	this->normal = TriangleNormal(this->vertices[0]->position, this->vertices[1]->position, this->vertices[2]->position);
}

HOST DEVICE Scalar MeshTriangle::Area() const {
	return (this->vertices[1]->position - this->vertices[0]->position).Cross(this->vertices[2]->position - this->vertices[0]->position).Length() / 2;
}

HOST DEVICE bool MeshTriangle::RayIntersect(const Ray &_ray, Intersection<MeshTriangle> &_intersection) const {
	// This equation is (_ray.origin + t * _ray.direction - plane.point) * plane.normal = 0.
	Scalar t = (this->vertices[0]->position - _ray.origin).Dot(this->normal) / _ray.direction.Dot(this->normal);
	if (t < SELF_INTERSECT_THRESHOLD) {
		return false;
	}
	Vector3 hit = _ray.Extend(t);
	// Make sure hit point is within triangle.
	for (int i = 0; i < 3; ++ i) {
		int ni = (i + 1) % 3;
		Vector3 side = this->vertices[ni]->position - this->vertices[i]->position;
		if (side.Cross(hit - this->vertices[i]->position).Dot(this->normal) < 0) {
			return false;
		}
	}
	_intersection.position = hit;
	_intersection.normal = this->normal;
	_intersection.element = this;
	return true;
}

HOST DEVICE Vector3 MeshTriangle::Center() const {
	return (this->vertices[0]->position + this->vertices[1]->position + this->vertices[2]->position) / 3;
}

HOST DEVICE Pair<Vector3, Vector3> MeshTriangle::Bound() const {
	Vector3 lower = Min(Min(this->vertices[0]->position, this->vertices[1]->position), this->vertices[2]->position);
	Vector3 upper = Max(Max(this->vertices[0]->position, this->vertices[1]->position), this->vertices[2]->position);
	return Pair<Vector3, Vector3>(lower, upper);
}

std::map<std::string, MeshData *> MeshData::DATA_;

HOST MeshData::MeshData(const std::string &_path) {
	this->load(_path);
	Vector<MeshTriangle *> pointers;
	pointers.Reserve(this->triangles_.Size());
	for (int i = 0; i < this->triangles_.Size(); ++ i) {
		pointers.PushBack(&this->triangles_[i]);
	}
	this->bvh_ = new BVH<MeshTriangle>(pointers, 0, pointers.Size());
}

HOST MeshData::~MeshData() {
	delete this->bvh_;
}

HOST MeshData *MeshData::Load(const std::string &_path) {
	// Create a new MeshData instance if _path not loaded and saved yet.
	if (MeshData::DATA_.find(_path) == MeshData::DATA_.end()) {
		MeshData::DATA_[_path] = new MeshData(_path);
	}
	return MeshData::DATA_[_path];
}

HOST void MeshData::load(const std::string &_path) {
	this->vertices_.Clear();
	this->triangles_.Clear();
	std::ifstream loader(_path);
	int vertex_count;
	loader >> vertex_count;
	this->vertices_.Reserve(vertex_count);
	for (int i = 0; i < vertex_count; ++i) {
		this->vertices_.PushBack(MeshVertex());
		Vector3 vertex;
		loader >> vertex.x >> vertex.y >> vertex.z;
		this->vertices_[i].position = vertex;
		this->vertices_[i].normal = Vector3(0, 0, 0);
	}
	int triangle_count;
	loader >> triangle_count;
	this->triangles_.Reserve(triangle_count);
	for (int i = 0; i < triangle_count; ++i) {
		this->triangles_.PushBack(MeshTriangle());
		int indices[3];
		for (int j = 0; j < 3; ++j) {
			loader >> indices[j];
			this->triangles_[i].vertices[j] = &this->vertices_[indices[j]];
		}
		this->triangles_[i].ComputeNormal();
		Vector3 weighted_normal = this->triangles_[i].normal * this->triangles_[i].Area();
		for (int j = 0; j < 3; ++j) {
			this->triangles_[i].vertices[j]->normal += weighted_normal;
		}
	}
	loader.close();
	for (int i = 0; i < vertex_count; ++ i) {
		this->vertices_[i].normal.Normalize();
	}
}

HOST Mesh::Mesh(
		Scene *_scene,
		const Pose &_pose,
		const std::string &_path,
		const bool _smooth,
		const MaterialBase *_material):
		ObjectBase(_scene, _pose, _material) {
	this->object_type_ = ObjectType::Mesh;
	this->data_ = MeshData::Load(_path);
	this->smooth_ = _smooth;
}

HOST DEVICE Vector3 Mesh::localCenter() const {
  return this->data_->bvh_->Center();
}

HOST DEVICE Pair<Vector3, Vector3> Mesh::localBound() const {
  return this->data_->bvh_->Bound();
}

HOST DEVICE bool Mesh::localRayIntersect(const Ray &_ray, Intersection<ObjectBase> &_intersection) const {
	bool intersect = false;
	_intersection.element = this;
	for (int i = 0; i < this->data_->triangles_.Size(); ++ i) {
		Intersection<MeshTriangle> triangle_intersection;
		if (this->data_->triangles_[i].RayIntersect(_ray, triangle_intersection)) {
			if (! intersect || _ray.Distance(triangle_intersection.position) < _ray.Distance(_intersection.position)) {
				_intersection.position = triangle_intersection.position;
				_intersection.normal = triangle_intersection.normal;
			}
			intersect = true;
		}
	}
	return intersect;

	// Intersection<MeshTriangle> mesh_intersection;
	// if (this->data_->bvh_->RayIntersect(_ray, mesh_intersection)) {
	// 	_intersection.position = mesh_intersection.position;
	// 	if (this->smooth_) {
	// 		const MeshTriangle *triangle = mesh_intersection.element;
	// 		Vector3 decomposition = TriangleDecomposite(
	// 			triangle->vertices[0]->position,
	// 			triangle->vertices[1]->position,
	// 			triangle->vertices[2]->position,
	// 			mesh_intersection.position);
	// 		_intersection.normal = (
	// 			triangle->vertices[0]->normal * decomposition.x +
	// 			triangle->vertices[1]->normal * decomposition.y +
	// 			triangle->vertices[2]->normal * decomposition.z).Normalized();
	// 	} else {
	// 		_intersection.normal = mesh_intersection.normal;
	// 	}
	// 	_intersection.element = this;
	// 	return true;
	// } else {
	// 	return false;
	// }
}
