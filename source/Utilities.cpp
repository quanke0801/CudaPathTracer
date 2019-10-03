#include "Utilities.h"

HOST DEVICE Scalar DegreeToRadian(const Scalar _degree) {
	return _degree / Scalar(180) * PI;
}

HOST DEVICE Scalar RadianToDegree(const Scalar _radian) {
	return _radian / PI * Scalar(180);
}

HOST DEVICE Vector3 TriangleNormal(const Vector3 &_point_1, const Vector3 &_point_2, const Vector3 &_point_3) {
	return (_point_2 - _point_1).Cross(_point_3 - _point_1).Normalized();
}

HOST DEVICE Vector3 TriangleDecomposite(
		const Vector3 &_point_1,
		const Vector3 &_point_2,
		const Vector3 &_point_3,
		const Vector3 &_point) {
	Vector3 decomposition;
	Vector3 normal = TriangleNormal(_point_1, _point_2, _point_3);
	Vector3 normal_13 = normal.Cross(_point_1 - _point_3);
	Vector3 normal_23 = normal.Cross(_point_2 - _point_3);
	decomposition.x = (_point - _point_3).Dot(normal_23) / (_point_1 - _point_3).Dot(normal_23);
	decomposition.y = (_point - _point_3).Dot(normal_13) / (_point_2 - _point_3).Dot(normal_13);
	decomposition.z = Scalar(1) - decomposition.x - decomposition.y;
	return decomposition;
}

HOST DEVICE void GenerateAxis(const Vector3 &_axis_1,
		Vector3 &_axis_2,
		Vector3 &_axis_3) {
	Vector3 pivot = Vector3(1, 0, 0);
	if (_axis_1.Cross(pivot).Length() < SCALAR_LOOSE_DELTA) {
		pivot = Vector3(0, 1, 0);
	}
	_axis_2 = _axis_1.Cross(pivot).Normalized();
	_axis_3 = _axis_1.Cross(_axis_2).Normalized();
}

HOST DEVICE void GenerateAxis(
  	const Vector3 &_axis_1, const Vector3 &_pivot, Vector3 &_axis_2, Vector3 &_axis_3) {
	_axis_3 = _axis_1.Cross(_pivot).Normalized();
	_axis_2 = _axis_3.Cross(_axis_1).Normalized();
}

HOST DEVICE Vector3 RotateVector(const Vector3 &_vector, const Vector3 &_axis, const Scalar _angle) {
	Vector3 axis = _axis.Normalized();
	Vector3 pivot = axis.Cross(_vector);
	if (pivot.Length() < SCALAR_LOOSE_DELTA) {
		return _vector;
	}
	Vector3 base_2 = pivot.Normalized();
	Vector3 base_1 = base_2.Cross(axis);
	Vector3 rotated_base = base_1 * std::cos(_angle) + base_2 * std::sin(_angle);
	return axis * _vector.Dot(axis) + rotated_base * _vector.Dot(base_1);
}

HOST int RandomInteger(const int _lower, const int _upper) {
	return rand() % (_upper - _lower + 1) + _lower;
}

HOST Scalar Random(const Scalar _lower, const Scalar _upper) {
	return Scalar(rand()) / Scalar(RAND_MAX) * (_upper - _lower) + _lower;
}

HOST DEVICE bool RayBoxIntersect(const Ray &_ray, const Pair<Vector3, Vector3> &_bound) {
	Vector3 lower_t = (_bound.first - _ray.origin) / _ray.direction;
	Vector3 upper_t = (_bound.second - _ray.origin) / _ray.direction;
	Vector3 enter_t = Min(lower_t, upper_t);
	Vector3 exit_t = Max(lower_t, upper_t);
	return exit_t.Min() >= enter_t.Max() && exit_t.Max() >= 0;
}

HOST DEVICE Vector3 Reflect(const Vector3 &_in, const Vector3 &_normal) {
	return (_in - 2 * _normal.Dot(_in) * _normal).Normalized();
}
