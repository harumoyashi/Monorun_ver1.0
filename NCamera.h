#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NCamera
{
private:
	//カメラ
	XMMATRIX matProjection;
	XMMATRIX matView;

	//カメラ
	XMFLOAT3 eye = { 0, 100, -1500 };	//視点座標
	XMFLOAT3 target = { 0, 0, 0 };		//注視点座標
	XMFLOAT3 up = { 0, 1, 0 };			//上方向ベクトル
	float angle = 0.0f;					//カメラの回転角

public:
	//ビュー変換行列作成
	void CreateMatView();
	//射影投影変換//
	void ProjectiveProjection();

	//セッター
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }
	void SetUp(XMFLOAT3 up) { this->up = up; }

	//ゲッター
	XMMATRIX GetMatProjection() { return matProjection; }
	XMMATRIX GetMatView() { return matView; }
};