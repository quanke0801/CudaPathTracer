#pragma once

#include "Common.h"

class Vector2 {
public:
	HOST DEVICE Vector2(const Scalar _x = 0, const Scalar _y = 0);
	// Properties.
	HOST DEVICE Scalar LengthSquare() const;
	HOST DEVICE Scalar Length() const;
	HOST DEVICE void Normalize();
	HOST DEVICE Vector2 Normalized() const;
	// Vector operations.
	HOST DEVICE Scalar Dot(const Vector2 &_other) const;
	HOST DEVICE Scalar Cross(const Vector2 &_other) const;
	// Data range information.
	HOST DEVICE Scalar Min() const;
	HOST DEVICE int MinIndex() const;
	HOST DEVICE Scalar Max() const;
	HOST DEVICE int MaxIndex() const;
	// Operators.
	HOST DEVICE Vector2 operator - () const;
	HOST DEVICE Vector2 operator + (const Vector2 &_other) const;
	HOST DEVICE Vector2 operator - (const Vector2 &_other) const;
	HOST DEVICE Vector2 operator * (const Scalar _value) const;
	HOST DEVICE Vector2 operator * (const Vector2 &_other) const;
	HOST DEVICE Vector2 operator / (const Scalar _value) const;
	HOST DEVICE Vector2 operator / (const Vector2 &_other) const;
	HOST DEVICE void operator += (const Vector2 &_other);
	HOST DEVICE void operator -= (const Vector2 &_other);
	HOST DEVICE void operator *= (const Scalar _value);
	HOST DEVICE void operator *= (const Vector2 &_other);
	HOST DEVICE void operator /= (const Scalar _value);
	HOST DEVICE void operator /= (const Vector2 &_other);
public:
	Scalar x, y;
};

HOST std::ostream &operator << (std::ostream &_stream, const Vector2 &_vector);
HOST DEVICE Vector2 operator * (const Scalar _value, const Vector2 &_vector);
HOST DEVICE Vector2 Min(const Vector2 &_vector_1, const Vector2 &_vector_2);
HOST DEVICE Vector2 Max(const Vector2 &_vector_1, const Vector2 &_vector_2);
