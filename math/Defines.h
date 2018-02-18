#pragma once

#include <stdint.h>

namespace apryx {

	template <typename T>
	struct Vector2;
	template <typename T>
	struct Vector3;
	template <typename T>
	struct Vector4;

	template <typename T>
	struct Matrix4;
	template <typename T>
	struct Matrix3;
	template <typename T>
	struct Matrix2;


	typedef Vector2<float> Vector2f;
	typedef Vector2<int> Vector2i;
	typedef Vector2<double> Vector2d;
	typedef Vector2<long long> Vector2ll;


	typedef Vector3<float> Vector3f;
	typedef Vector3<int> Vector3i;
	typedef Vector3<double> Vector3d;
	typedef Vector3<long long> Vector3ll;

	typedef Vector4<float> Vector4f;
	typedef Vector4<int> Vector4i;
	typedef Vector4<double> Vector4d;

	typedef Vector4<float> Color;
	typedef Vector4<uint8_t> Color32;

	typedef Matrix2<float> Matrix2f;
	typedef Matrix2<double> Matrix2d;
	typedef Matrix2<int> Matrix2i;
	typedef Matrix2<long long> Matrix2ll;

	typedef Matrix3<float> Matrix3f;
	typedef Matrix3<double> Matrix3d;
	typedef Matrix3<int> Matrix3i;
	typedef Matrix3<long long> Matrix3ll;

	typedef Matrix4<float> Matrix4f;
	typedef Matrix4<double> Matrix4d;
}