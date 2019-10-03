#include "BVH.h"

#include "Mesh.h"
#include "ObjectBase.h"

template <typename T>
const Vector3 BVH<T>::SPLIT_AXIS[3] = {Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1)};

template <typename T>
HOST BVH<T>::BVH() {
  this->leaf_ = true;
  this->bound_.first = Vector3(0, 0, 0);
  this->bound_.second = Vector3(0, 0, 0);
  this->parent_ = nullptr;
  this->children_[0] = nullptr;
  this->children_[1] = nullptr;
}

template <typename T>
HOST BVH<T>::BVH(Vector<T *> &_elements, const int _begin, const int _end, BVH<T> *_parent) {
  this->parent_ = _parent;
  if (_end - _begin <= BVH<T>::MAX_LEAF_SIZE) {
    // Build leaf node.
    this->leaf_ = true;
    // Copy elements to member.
    this->elements_.Reserve(BVH<T>::MAX_LEAF_SIZE);
    for (int i = _begin; i < _end; ++ i) {
      this->elements_.PushBack(_elements[i]);
    }
    this->updateBoundFromElements();
    this->children_[0] = nullptr;
    this->children_[1] = nullptr;
  } else {
    // Build intermediate node.
    this->leaf_ = false;
    this->split(_elements, _begin, _end);
  }
}

template <typename T>
HOST BVH<T>::~BVH() {
  // Recursively delete entire tree.
  if (! this->leaf_) {
    delete this->children_[0];
    delete this->children_[1];
  }
}

template <typename T>
HOST void BVH<T>::Insert(T *_element) {
  if (this->leaf_) {
    this->elements_.PushBack(_element);
    if (this->elements_.Size() <= MAX_LEAF_SIZE) {
      // Leaf is not full, then simply insert.
      this->updateBoundFromElements();
    } else {
      // Leaf is full, then split.
      this->split(this->elements_, 0, this->elements_.Size());
    }
  } else {
    if (this->split_axis_.Dot(_element->Center()) < this->split_value_) {
      this->children_[0]->Insert(_element);
    } else {
      this->children_[1]->Insert(_element);
    }
    this->updateBoundFromChildren();
  }
}

template <typename T>
HOST DEVICE bool BVH<T>::RayIntersect(const Ray &_ray, Intersection<T> &_intersection) const {
  // Fast coarse test intersection with AABB.
  if (! RayBoxIntersect(_ray, this->bound_)) {
    return false;
  }
  // Finer test.
  bool intersect = false;
  if (this->leaf_) {
    // Test intersection with elements for leaf nodes.
    for (int i = 0; i < this->elements_.Size(); ++ i) {
      Intersection<T> element_intersection;
      if (this->elements_[i]->RayIntersect(_ray, element_intersection)) {
        // Update intersection result if first hit or nearer hit.
        if (! intersect || _ray.Distance(element_intersection.position) < _ray.Distance(_intersection.position)) {
          _intersection = element_intersection;
        }
        intersect = true;
      }
    }
  } else {
    // Test intersection with children nodes.
    for (int i = 0; i < 2; ++ i) {
      Intersection<T> child_intersection;
      if (this->children_[i]->RayIntersect(_ray, child_intersection)) {
        // Update intersection result if first hit or nearer hit.
        if (! intersect || _ray.Distance(child_intersection.position) < _ray.Distance(_intersection.position)) {
          _intersection = child_intersection;
        }
        intersect = true;
      }
    }
  }
  return intersect;
}

template <typename T>
HOST DEVICE Vector3 BVH<T>::Center() const {
  return (this->bound_.first + this->bound_.second) / 2;
}

template <typename T>
HOST DEVICE Pair<Vector3, Vector3> BVH<T>::Bound() const {
  return this->bound_;
}

template <typename T>
HOST BVH<T> *BVH<T>::SearchElementLeaf(const T *_element) {
  if (this->leaf_) {
    return this;
  }
  if (this->split_axis_.Dot(_element->Center()) < this->split_value_) {
    return this->children_[0]->SearchElementLeaf(_element);
  } else {
    return this->children_[1]->SearchElementLeaf(_element);
  }
}

template <typename T>
HOST void BVH<T>::UpdateBoundBottomUp() {
  if (this->leaf_) {
    this->updateBoundFromElements();
  } else {
    this->updateBoundFromChildren();
  }
  if (this->parent_ != nullptr) {
    this->parent_->UpdateBoundBottomUp();
  }
}

template <typename T>
HOST void BVH<T>::split(Vector<T *> &_elements, const int _begin, const int _end) {
  // Compute variance in x, y, z direction.
  Vector3 sum(0, 0, 0);
  Vector3 square_sum(0, 0, 0);
  for (int i = _begin; i < _end; ++ i) {
    const Vector3 center = _elements[i]->Center();
    sum += center;
    square_sum += (center * center);
  }
  Vector3 variance = square_sum * (_end - _begin) - sum * sum;
  // Pick axis with largest variance to divide space.
  this->split_axis_ = BVH<T>::SPLIT_AXIS[variance.MaxIndex()];
  int mid = _begin + (_end - _begin) / 2;
  std::nth_element(
    _elements.Data() + _begin,
    _elements.Data() + mid,
    _elements.Data() + _end,
    [&](const T *_1, const T *_2) {
      return this->split_axis_.Dot(_1->Center()) < this->split_axis_.Dot(_2->Center());
    });
  this->split_value_ = this->split_axis_.Dot(_elements[mid]->Center());
  // Create 2 children nodes.
  this->children_[0] = new BVH<T>(_elements, _begin, mid, this);
  this->children_[1] = new BVH<T>(_elements, mid, _end, this);
  this->leaf_ = false;
  this->elements_.Clear();
  // Compute bounding box from children.
  this->updateBoundFromChildren();
}

template <typename T>
HOST void BVH<T>::updateBoundFromElements() {
  this->bound_ = this->elements_[0]->Bound();
  for (int i = 0; i < this->elements_.Size(); ++ i) {
    Pair<Vector3, Vector3> bound = this->elements_[i]->Bound();
    this->bound_.first = Min(this->bound_.first, bound.first);
    this->bound_.second = Max(this->bound_.second, bound.second);
  }
}

template <typename T>
HOST void BVH<T>::updateBoundFromChildren() {
  this->bound_.first = Min(this->children_[0]->bound_.first, this->children_[1]->bound_.first);
  this->bound_.second = Max(this->children_[0]->bound_.second, this->children_[1]->bound_.second);
}

template class BVH<MeshTriangle>;
template class BVH<ObjectBase>;
