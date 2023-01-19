#include "Vectors.h"
#include "Math.h"
#include <math.h>
#include <cassert>

Vector2D::Vector2D(float x, float y, float w) {
	this->x = x;
	this->y = y;
	this->w = w;
}

float Vector2D::Magnitude() {
	return sqrtf(x * x + y * y);
}

void Vector2D::Normalize() {
	const float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
}

Vector2D Vector2D::operator+(const Vector2D& other) {
	return{ this->x + other.x, this->y + other.y, this->w };
}

void Vector2D::operator*=(const float& scale) {
	this->x *= scale;
	this->y *= scale;
}

Vector3::Vector3(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
	return Vector3{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

Vector3 Vector3::operator-(const Vector3 other) {
	return Vector3{ x - other.x, y - other.y, z - other.z };
}