#include "Math.h"

Vector3 Matrix::Transform(const Vector3& vector) {
	Vector3 result{0,0,0};
	result.x = data[0].x * vector.x + data[0].y * vector.y + data[0].z * vector.z + data[0].w * vector.w;
	result.y = data[1].x * vector.x + data[1].y * vector.y + data[1].z * vector.z + data[1].w * vector.w;
	result.z = data[2].x * vector.x + data[2].y * vector.y + data[2].z * vector.z + data[2].w * vector.w;
	result.w = data[3].x * vector.x + data[3].y * vector.y + data[3].z * vector.z + data[3].w * vector.w;

	return result;
}