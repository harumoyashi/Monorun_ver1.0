#include "NVector3.h"
#include <cmath>

NVector3::NVector3() {

}

NVector3::NVector3(float x, float y, float z) :x(x), y(y), z(z) {

}

float NVector3::Length() const {
	return sqrtf(x * x + y * y + z * z);
}

NVector3& NVector3::Normalize() {
	float len = Length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

NVector3 NVector3::Cross(const NVector3& v) const
{
	NVector3 vec3;
	vec3.x = y * v.z - z * v.y;
	vec3.y = z * v.x - x * v.z;
	vec3.z = x * v.y - y * v.x;

	return vec3;
}

float NVector3::Dot(const NVector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

NVector3 NVector3::operator+()const
{
	return *this;
}

NVector3 NVector3::operator-()const
{
	return NVector3(-x, -y, -z);
}

NVector3 NVector3::operator*(const NVector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

NVector3& NVector3::operator+=(const NVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

NVector3& NVector3::operator-=(const NVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

NVector3& NVector3::operator*=(const NVector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

NVector3& NVector3::operator/=(const NVector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

NVector3& NVector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

NVector3& NVector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//NVector3クラスに属さない関数群
// 二項演算子
const NVector3 operator+(const NVector3& v1, const NVector3& v2)
{
	NVector3 temp(v1);
	return temp += v2;
}

const NVector3 operator-(const NVector3& v1, const NVector3& v2)
{
	NVector3 temp(v1);
	return temp -= v2;
}

const NVector3 operator*(const NVector3& v, float s)
{
	NVector3 temp(v);
	return temp *= s;
}

const NVector3 operator*(float s, const NVector3& v)
{
	return v * s;
}

const NVector3 operator/(const NVector3& v, float s)
{
	NVector3 temp(v);
	return temp /= s;
}

float Dot(const NVector3& v, const NVector3& v2)
{
	return v.x * v2.x + v.y * v2.y + v.z * v2.z;
}

float Degree2Radian(float degree)
{
	return degree * PI / 180;
}

float Radian2Degree(float radian)
{
	return radian * 180 / PI;
}

int Sign(int value)
{
	if (value > 0)
	{
		return 1;
	}
	else if (value < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

float Sign(float value)
{
	if (value > 0.0f)
	{
		return 1.0f;
	}
	else if (value < 0.0f)
	{
		return -1.0f;
	}
	else
	{
		return 0.0f;
	}
}

float Clamp(float value, float max, float min)
{
	// 値が最大値を上回っていたら最大値を返す
	if (value >= max) return max;

	// 値が最小値を下回っていたら最小値を返す
	if (value <= min) return min;

	// どちらにも当てはまらなかったら値をそのまま返す
	return value;
}