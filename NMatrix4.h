#pragma once
#include "NVector3.h"

struct NMatrix4
{
	float m[4][4];
};

//単位行列
NMatrix4 Identity();
//拡縮行列の設定
NMatrix4 Scale(const NVector3& s);
//回転行列の設定
NMatrix4 RotateX(float angle);
NMatrix4 RotateY(float angle);
NMatrix4 RotateZ(float angle);
//平行移動行列の作成
NMatrix4 Translation(const NVector3& t);
//座標変換（ベクトルと行列の掛け算）
NVector3 Transform(const NVector3& v, const NMatrix4& m);
//w除算を使った座標変換
NVector3 WTransform(const NVector3& v, const NMatrix4& m);

//代入演算子オーバーロード
NMatrix4& operator*=(NMatrix4& m1, const NMatrix4& m2);

//2項演算子オーバーロード
const NMatrix4 operator*(const NMatrix4& m1, const NMatrix4& m2);
const NVector3 operator*(const NVector3& v, const NMatrix4& m);