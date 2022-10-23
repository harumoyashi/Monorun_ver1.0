#include "NTitleScene.h"
#include "NSceneManager.h"
#include "NInput.h"

NTitleScene* NTitleScene::GetInstance()
{
	static NTitleScene instance;
	return &instance;
}

void NTitleScene::Initialize(NDX12* dx12)
{
#pragma region 描画初期化処理
	//マテリアル(定数バッファ)
	material.Initialize(dx12->GetDevice());

	//オブジェクト(定数バッファ)
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].Initialize(dx12->GetDevice());
		obj3d[i].texNum = PLAYER;
	}

	obj3d[0].scale = { 24.0f,24.0f,24.0f };
	obj3d[0].position = { 0.0f,-100.0f,0.0f };

	////背景スプライト生成
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i] = new NSprite();
	//	backSprite[i]->texNum = static_cast<int>(i);
	//	backSprite[i]->CreateSprite(dx12->GetDevice(), tex[backSprite[i]->texNum].texBuff);
	//	//sprite[i]->CreateClipSprite(dx12->GetDevice(),tex[sprite[i]->texNum].texBuff,{100.0f,0},{50.0f,100.0f});	//一部切り取って生成
	//	backSprite[i]->position.x = i * 300.0f + 400.0f;
	//	backSprite[i]->position.y = 400.0f;
	//	backSprite[i]->UpdateMatrix();
	//	backSprite[i]->TransferMatrix();
	//}

	//前景スプライト生成
	titleSprite = new NSprite();
	titleSprite->texNum = TITLE;
	titleSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[titleSprite->texNum].texBuff);
	titleSprite->position = { NWindows::win_width / 2,NWindows::win_height / 2,0 };
	titleSprite->UpdateMatrix();

	startSprite = new NSprite();
	startSprite->texNum = START;
	startSprite->CreateSprite(dx12->GetDevice(), NSceneManager::GetTex()[titleSprite->texNum].texBuff);
	startSprite->position = { NWindows::win_width / 2,NWindows::win_height / 2,0 };
	startSprite->UpdateMatrix();

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

void NTitleScene::Update()
{
	if (NInput::IsKeyTrigger(DIK_SPACE))
	{
		NSceneManager::SetScene(STAGESELECTSCENE);
	}
#pragma region 行列の計算
	obj3d[0].rotation.z += 0.3f;
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].UpdateMatrix(matView, matProjection);
	}
#pragma endregion
}

void NTitleScene::Draw(NDX12* dx12)
{
#pragma region グラフィックスコマンド
	// 4.描画コマンドここから
	////背景スプライト
	//for (size_t i = 0; i < maxBackSprite; i++)
	//{
	//	backSprite[i]->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState, NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	//	backSprite[i]->Draw(dx12->GetSRVHeap(), tex[0].incrementSize, dx12->GetCommandList());
	//}

	//3Dオブジェクト
	for (size_t i = 0; i < maxObj; i++)
	{
		obj3d[i].CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipeline3d()->pipelineSet.pipelineState,
			NSceneManager::GetPipeline3d()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
		obj3d[i].Draw(dx12->GetCommandList(), material, dx12->GetSRVHeap(), NSceneManager::GetPipeline3d()->vbView, NSceneManager::GetPipeline3d()->ibView, NSceneManager::GetPipeline3d()->numIB, NSceneManager::GetTex()[0].incrementSize);
	}

	//前景スプライト
	titleSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
		NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	titleSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

	startSprite->CommonBeginDraw(dx12->GetCommandList(), NSceneManager::GetPipelineSprite()->pipelineSet.pipelineState,
		NSceneManager::GetPipelineSprite()->pipelineSet.rootSig.entity, dx12->GetSRVHeap());
	startSprite->Draw(dx12->GetSRVHeap(), NSceneManager::GetTex()[0].incrementSize, dx12->GetCommandList());

	// 4.描画コマンドここまで
#pragma endregion
}

void NTitleScene::Finalize()
{
	delete titleSprite;

	/*for (size_t i = 0; i < maxBackSprite; i++)
	{
		delete backSprite[i];
	}*/
}
