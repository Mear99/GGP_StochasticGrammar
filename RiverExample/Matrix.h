#pragma once

struct Matrix {
	Vector3 data[4]{
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1},
	};

	Vector3 Transform(const Vector3& vector);
};