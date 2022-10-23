#include "NGameScene.h"
#include "NSceneManager.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

void NGameScene::Initialize(NDX12* dx12)
{
	//マテリアル(定数バッファ)
	material_.Initialize(dx12->GetDevice());

	// --プレイヤー初期化-- //
	player_ = player_->GetInstance();
	player_->Initialize(dx12);

	stage_ = stage_->GetInstance();
	stage_->Initialize(dx12);

	//射影投影変換//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//上下画角45度
		(float)NWindows::win_width / NWindows::win_height,	//アスペクト比(画面横幅/画面縦幅)
		0.1f, 2000.0f					//前端、奥端
	);

	eye = { 0, 100, -1500 };	//視点座標
	target = { 0, 0, 0 };	//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル

	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void NGameScene::Update()
{
	// --プレイヤー更新処理-- //
	player_->Update(matView, matProjection, eye, target, up);

	stage_->Update(matView, matProjection, eye, target, up);
}

void NGameScene::Draw(NDX12* dx12)
{
	// --プレイヤー描画処理-- //
	player_->Draw(dx12, material_);

	stage_->Draw(dx12);
}

// --リセット処理-- //
void NGameScene::Reset(NDX12* dx12) {
	stage_->LoadCSV(dx12);
}

void NGameScene::Finalize()
{
	stage_->Finalize();
	stage_->Release();
	player_->Finalize();
	/*for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}