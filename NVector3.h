#pragma once
#include <DirectXMath.h>

const float PI = 3.141592654f;

class NVector3
{
public:
	float x;
	float y;
	float z;

public:
	NVector3();							//零ベクトルとする
	NVector3(float x, float y, float z);	//x成分,y成分,z成分を指定しての生成

	//メンバ変数
	float Length() const;					//ノルム(長さ)を求める
	NVector3& Normalize();					//正規化をする
	float Dot(const NVector3& v) const;		//内籍を求める
	NVector3 Cross(const NVector3& v) const;	//外積を求める

	//単項演算子オーバーロード
	NVector3 operator+() const;
	NVector3 operator-() const;
	NVector3 operator*(const NVector3& v);

	//代入演算子オーバーロード
	NVector3& operator+=(const NVector3& v);
	NVector3& operator-=(const NVector3& v);
	NVector3& operator*=(const NVector3& v);
	NVector3& operator/=(const NVector3& v);
	NVector3& operator*=(float s);
	NVector3& operator/=(float s);
};

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const NVector3 operator+(const NVector3& v1, const NVector3& v2);
const NVector3 operator-(const NVector3& v1, const NVector3& v2);
const NVector3 operator*(const NVector3& v, float s);
const NVector3 operator*(float s, const NVector3& v);
const NVector3 operator/(const NVector3& v, float s);

float Dot(const NVector3& v, const NVector3& v2);	//内籍を求める
//度数法→ラジアン
float Degree2Radian(float degree);
//度数法→ラジアン
float Radian2Degree(float radian);
//符号返す
int Sign(int value);
//符号返す
float Sign(float value);
//最小、最大値設定
float Clamp(float value, float max, float min);