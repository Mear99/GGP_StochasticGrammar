#pragma once

struct Vector2D {
	float x, y;
	float w;	// w = 1 -> point, w = 0 -> vector

	Vector2D(float x, float y, float w = 1);

	float Magnitude() ;
	void Normalize();


	Vector2D operator+(const Vector2D& other);
	void operator*=(const float& other);

};

struct Vector3 {
	float x, y, z;
	float w; // w = 1 -> point, w = 0 -> vector

	Vector3(float x, float y, float z, float w = 1);
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);

	Vector3 operator-(const Vector3 other);
};
