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

	int shakeCount_{ 0 };

	XMFLOAT3 totalEye{ };
	XMFLOAT3 totalTarget{};

public:
	//ビュー変換行列作成
	void CreateMatView();
	void CreateShakeMatView(float value, float value2);
	//射影投影変換//
	void ProjectiveProjection();
	// --縦スクロールのスピード取得-- //
	void SetScrollY(float scrollY);

	void SetShakeCount(int count) { shakeCount_ = count; }

	//セッター
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	void SetTarget(XMFLOAT3 target) { this->target = target; }
	void SetUp(XMFLOAT3 up) { this->up = up; }

	//ゲッター
	XMMATRIX GetMatProjection() { return matProjection; }
	XMMATRIX GetMatView() { return matView; }
	XMFLOAT3 GetEye() { return eye; }
	XMFLOAT3 GetTarget() { return target; }
	int GetShakeCount(void) { return shakeCount_; }
};