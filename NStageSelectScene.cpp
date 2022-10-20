#include "NStageSelectScene.h"
#include "NSceneManager.h"
#include "NInput.h"

NStageSelectScene* NStageSelectScene::GetInstance()
{
	static NStageSelectScene instance;
	return &instance;
}

void NStageSelectScene::Initialize(NDX12* dx12)
{
#pragma region 描画初期化処理
	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());

	//オブジェクト(定数バッファ)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->GetDevice());
		obj3d[i].texNum = 1;
	}

	obj3d[1].position.x = -20.0f;
	obj3d[2].position.x = 20.0f;

	//背景スプライト生成
	for (size_t i = 0; i < maxBackSprite; i++)
	{
		backSprite[i] = new NSprite();
		backSprite[i]->texNum = static_cast<int>(i);
		backSprite[i]->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[backSprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
		backSprite[i]->position.x = i * 300.0f + 400.0f;
		backSprite[i]->position.y = 400.0f;
		backSprite[i]->UpdateMatrix();
		backSprite[i]->TransferMatrix();
	}

	//前景スプライト生成
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i] = new NSprite();
		foreSprite[i]->texNum = static_cast<int>(i);
		foreSprite[i]->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[foreSprite[i]->texNum].texBuff);
		//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
		foreSprite[i]->SetColor(1, 1, 1, 0.5f);
		foreSprite[i]->size = { 150,150 };
		foreSprite[i]->TransferVertex();
		foreSprite[i]->position.x = i * 200.0f + 200.0f;
		foreSprite[i]->position.y = 200.0f;
		foreSprite[i]->UpdateMatrix();
		foreSprite[i]->TransferMatrix();
	}
#pragma endregion
#pragma region	カメラ初期化
	//射影投影変換//
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),		//上下画角45度
		(float)NWindows::win_width / NWindows::win_height,	//アスペクト比(画面横幅/画面縦幅)
		0.1f, 2000.0f					//前端、奥端
	);

	//ここでビュー変換行列計算
	matView;
	eye = { -100, 100, -1000 };	//視点座標
	target = { 0, 0, 0 };	//注視点座標
	up = { 0, 1, 0 };		//上方向ベクトル
	//ビュー変換行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	angle = 0.0f;	//カメラの回転角
#pragma endregion
}

void NStageSelectScene::Update()
{
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		NSceneManager::SetScene(GAMESCENE);
	}
#pragma region 行列の計算
	//ビュー変換行列再作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	obj3d[0].MoveKey();
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].UpdateMatrix();
		obj3d[i].TransferMatrix(matView, matProjection);
	}
#pragma endregion
}

void NStageSelectScene::Draw(NDX12* dx12)
{
#pragma region グラフィックスコマンド
	// 4.描画コマンドここから
	//背景スプライト
	for (size_t i = 0; i < maxBackSprite; i++)
	{
		backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		backSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}

	//3Dオブジェクト
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState, NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
	}

	//前景スプライト
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		foreSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		foreSprite[i]->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());
	}
	// 4.描画コマンドここまで
#pragma endregion
}

void NStageSelectScene::Finalize()
{
	for (size_t i = 0; i < maxForeSprite; i++)
	{
		delete foreSprite[i];
	}

	for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}
}