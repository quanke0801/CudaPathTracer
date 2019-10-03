#pragma once

#include "Common.h"
#include "Vector3.h"
#include "Ray.h"
#include "Utilities.h"

// Boundary volume hierarchy for: 1, Scene with objects; 2, Mesh with triangles.
// The implementation is a binary tree with AABB and max-variance-axis split.
template <typename T>
class BVH: public Managed {
public:
  HOST BVH();
  // Build BVH with vector of elements. Elements will be sorted in the process.
  HOST BVH(
    Vector<T *> &_elements,
    const int _begin,
    const int _end,
    BVH<T> *_parent = nullptr);
  HOST ~BVH();
  HOST void Insert(T *_element);
  HOST DEVICE bool RayIntersect(const Ray &_ray, Intersection<T> &_intersection) const;
  HOST DEVICE Vector3 Center() const;
  HOST DEVICE Pair<Vector3, Vector3> Bound() const;
  // Search for the leaf node containing given element.
  HOST BVH<T> *SearchElementLeaf(const T *_element);
  // After an element changes, update bvh nodes from bottom up.
  HOST void UpdateBoundBottomUp();
private:
  HOST void split(Vector<T *> &_elements, const int _begin, const int _end);
  HOST void updateBoundFromElements();
  HOST void updateBoundFromChildren();
private:
  static const int MAX_LEAF_SIZE = 4;
  static const Vector3 SPLIT_AXIS[3];
  bool leaf_;
  Pair<Vector3, Vector3> bound_;
  BVH<T> *parent_;
  BVH<T> *children_[2];
  // Split criteria in intermediate nodes.
  Vector3 split_axis_;
  Scalar split_value_;
  // Elements in leaf nodes.
  Vector<T *> elements_;
};
