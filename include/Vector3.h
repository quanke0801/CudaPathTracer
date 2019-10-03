#pragma once

#include "Common.h"

class Vector3 {
public:
	HOST DEVICE Vector3(const Scalar _x = 0, const Scalar _y = 0, const Scalar _z = 0);
	// Properties.
	HOST DEVICE Scalar LengthSquare() const;
	HOST DEVICE Scalar Length() const;
	HOST DEVICE void Normalize();
	HOST DEVICE Vector3 Normalized() const;
	// Vector operations.
	HOST DEVICE Scalar Dot(const Vector3 &_other) const;
	HOST DEVICE Vector3 Cross(const Vector3 &_other) const;
	// Data range information.
	HOST DEVICE Scalar Min() const;
	HOST DEVICE int MinIndex() const;
	HOST DEVICE Scalar Max() const;
	HOST DEVICE int MaxIndex() const;
	// Operators.
	HOST DEVICE Vector3 operator - () const;
	HOST DEVICE Vector3 operator + (const Vector3 &_other) const;
	HOST DEVICE Vector3 operator - (const Vector3 &_other) const;
	HOST DEVICE Vector3 operator * (const Scalar _value) const;
	HOST DEVICE Vector3 operator * (const Vector3 &_other) const;
	HOST DEVICE Vector3 operator / (const Scalar _value) const;
	HOST DEVICE Vector3 operator / (const Vector3 &_other) const;
	HOST DEVICE void operator += (const Vector3 &_other);
	HOST DEVICE void operator -= (const Vector3 &_other);
	HOST DEVICE void operator *= (const Scalar _value);
	HOST DEVICE void operator *= (const Vector3 &_other);
	HOST DEVICE void operator /= (const Scalar _value);
	HOST DEVICE void operator /= (const Vector3 &_other);
public:
	Scalar x, y, z;
};

HOST std::ostream &operator << (std::ostream &_stream, const Vector3 &_vector);
HOST DEVICE Vector3 operator * (const Scalar _value, const Vector3 &_vector);
HOST DEVICE Vector3 Min(const Vector3 &_vector_1, const Vector3 &_vector_2);
HOST DEVICE Vector3 Max(const Vector3 &_vector_1, const Vector3 &_vector_2);

HOST DEVICE void Print(const Vector3 &_v);
