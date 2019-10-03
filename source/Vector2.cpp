#include "Vector2.h"

HOST DEVICE Vector2::Vector2(const Scalar _x, const Scalar _y):
		x(_x),
		y(_y) {
}

HOST DEVICE Scalar Vector2::LengthSquare() const {
	return this->x * this->x + this->y * this->y;
}

HOST DEVICE Scalar Vector2::Length() const {
	return std::sqrt(this->LengthSquare());
}

HOST DEVICE void Vector2::Normalize() {
	auto length = this->Length();
	this->x /= length;
	this->y /= length;
}

HOST DEVICE Vector2 Vector2::Normalized() const {
	Vector2 normalized(this->x, this->y);
	normalized.Normalize();
	return normalized;
}

HOST DEVICE Scalar Vector2::Dot(const Vector2 &_other) const {
	return this->x * _other.x + this->y * _other.y;
}

HOST DEVICE Scalar Vector2::Cross(const Vector2 &_other) const {
	return this->x * _other.y - this->y * _other.x;
}

HOST DEVICE Scalar Vector2::Min() const {
	return this->x < this->y ? this->x : this->y;
}

HOST DEVICE int Vector2::MinIndex() const {
	return this->x < this->y ? 0 : 1;
}

HOST DEVICE Scalar Vector2::Max() const {
	return this->x < this->y ? this->y : this->x;
}

HOST DEVICE int Vector2::MaxIndex() const {
	return this->x < this->y ? 1 : 0;
}

HOST DEVICE Vector2 Vector2::operator - () const {
	return Vector2(-this->x, -this->y);
}

HOST DEVICE Vector2 Vector2::operator + (const Vector2 &_other) const {
	return Vector2(this->x + _other.x, this->y + _other.y);
}

HOST DEVICE Vector2 Vector2::operator - (const Vector2 &_other) const {
	return Vector2(this->x - _other.x, this->y - _other.y);
}

HOST DEVICE Vector2 Vector2::operator * (const Scalar _value) const {
	return Vector2(this->x * _value, this->y * _value);
}

HOST DEVICE Vector2 Vector2::operator * (const Vector2 &_other) const {
	return Vector2(this->x * _other.x, this->y * _other.y);
}

HOST DEVICE Vector2 Vector2::operator / (const Scalar _value) const {
	return Vector2(this->x / _value, this->y / _value);
}

HOST DEVICE Vector2 Vector2::operator / (const Vector2 &_other) const {
	return Vector2(this->x / _other.x, this->y / _other.y);
}

HOST DEVICE void Vector2::operator += (const Vector2 &_other) {
	this->x += _other.x;
	this->y += _other.y;
}

HOST DEVICE void Vector2::operator -= (const Vector2 &_other) {
	this->x -= _other.x;
	this->y -= _other.y;
}

HOST DEVICE void Vector2::operator *= (const Scalar _value) {
	this->x *= _value;
	this->y *= _value;
}

HOST DEVICE void Vector2::operator *= (const Vector2 &_other) {
	this->x *= _other.x;
	this->y *= _other.y;
}

HOST DEVICE void Vector2::operator /= (const Scalar _value) {
	this->x /= _value;
	this->y /= _value;
}

HOST DEVICE void Vector2::operator /= (const Vector2 &_other) {
	this->x /= _other.x;
	this->y /= _other.y;
}

HOST std::ostream &operator << (std::ostream &_stream, const Vector2 &_vector) {
	_stream << "(" << _vector.x << ", " << _vector.y << ")";
	return _stream;
}

HOST DEVICE Vector2 operator * (const Scalar _value, const Vector2 &_vector) {
	return Vector2(_value * _vector.x, _value * _vector.y);
}

HOST DEVICE Vector2 Min(const Vector2 &_vector_1, const Vector2 &_vector_2) {
	return Vector2(_vector_1.x < _vector_2.x ? _vector_1.x : _vector_2.x, _vector_1.y < _vector_2.y ? _vector_1.y : _vector_2.y);
}

HOST DEVICE Vector2 Max(const Vector2 &_vector_1, const Vector2 &_vector_2) {
	return Vector2(_vector_1.x < _vector_2.x ? _vector_2.x : _vector_1.x, _vector_1.y < _vector_2.y ? _vector_2.y : _vector_1.y);
}
